#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>


#define N 40000 // Grid size
#define STEPS 200 // Time steps
#define C 1.0   // Wave speed
#define DT 0.01 // Time step
#define DX 1.0  // Grid spacing

double u[N], u_prev[N];

void initialize(int start, int end) {
    for (int i = start; i < end; i++) {
        u[i] = sin(2.0 * M_PI * i / N);
        u_prev[i] = u[i];
    }
}

void compute_step(int start, int end, int rank, int nprocs) {
    int chunk_size = end - start;

    // exchange data with neighbors
    // send data to the left neighbor
    int left_neighbor = (rank == 0) ? MPI_PROC_NULL : rank - 1;
    int right_neighbor = (rank == nprocs - 1) ? MPI_PROC_NULL : rank + 1;
    MPI_Sendrecv(&u[start], // send buffer
                    1, // send count
                    MPI_DOUBLE, // send type
                    left_neighbor, // destination rank
                    0, // destination tag
                    &u[start + chunk_size], // receive buffer
                    1, // receive count
                    MPI_DOUBLE, // receive type
                    right_neighbor, // source rank
                    0, // source tag
                    MPI_COMM_WORLD, // communicator
                    MPI_STATUS_IGNORE); // status
    
    // send data to the right neighbor
    MPI_Sendrecv(&u[start + chunk_size - 1], // send buffer
                    1, // send count
                    MPI_DOUBLE, // send type
                    right_neighbor, // destination rank
                    1, // destination tag
                    &u[start - 1], // receive buffer
                    1, // receive count
                    MPI_DOUBLE, // receive type
                    left_neighbor, // source rank
                    1, // source tag
                    MPI_COMM_WORLD, // communicator
                    MPI_STATUS_IGNORE); // status
    

    int compute_start = (start == 0) ? start + 1 : start;
    int compute_end = (end == N) ? end - 1 : end;
    double u_next[N];
    for (int i = compute_start; i < compute_end; i++) {
        u_next[i] = 2.0 * u[i] - u_prev[i] + C * C * DT * DT / (DX * DX) * (u[i+1] - 2.0 * u[i] + u[i-1]);
    }
    for (int i = start; i < end; i++) {
        u_prev[i] = u[i];
        u[i] = u_next[i];
    }
}

void write_output(int step, int rank, int start, int end, int nprocs) {
    // Gather all data to rank 0
    int recvcounts[nprocs];
    int displs[nprocs];
    int chunk_size = N / nprocs;
    for (int i = 0; i < nprocs; i++) {
        recvcounts[i] = chunk_size;
        displs[i] = i * chunk_size;
    }
    recvcounts[nprocs - 1] = N - (nprocs - 1) * chunk_size;
    
    double* u_all = NULL;
    if (rank == 0) {
        u_all = malloc(N * sizeof(double));
    }

    MPI_Gatherv(&u[start], // send buffer
                end - start, // send count
                MPI_DOUBLE, // send type
                u_all, // receive buffer
                recvcounts, // receive counts
                displs, // displacements
                MPI_DOUBLE, // receive type
                0, // root rank
                MPI_COMM_WORLD); // communicator


    // Only rank 0 writes the output
    if (rank != 0) {
        return;
    }
    // Write the output to a file 
    char filename[50];
    sprintf(filename, "wave_output_%d.txt", step);
    FILE *f = fopen(filename, "w");
    for (int i = 0; i < N; i++) {
        fprintf(f, "%f\n", u_all[i]);
    }
    free(u_all);
    fclose(f);
}

int main(int argc, char** argv) {
    int rank, nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    printf("MPI rank %d out of %d processes bb.\n", rank, nprocs);

    double start_time = MPI_Wtime();

    int start = rank * (N / nprocs);
    int end = (rank + 1) * (N / nprocs);
    end = (rank == nprocs - 1) ? N : end; 

    initialize(start, end);
    // if (rank == 0) {
    //     printf("Initialization complete.\n");
    // }

    for (int step = 0; step < STEPS; step++) {
        compute_step(start, end, rank, nprocs);
        // if (rank == 0) {
        //     printf("Step %d completed\n", step);
        // }
        if (step % 10 == 0) write_output(step, rank, start, end, nprocs);
    }

    double end_time = MPI_Wtime(); 
    double elapsed = end_time - start_time;
    MPI_Finalize();

    if(rank==0){
        printf("Simulation complete.\n");
        printf("Simulation complete. Total time: %.6f seconds\n", elapsed);
    }
    return 0;
}