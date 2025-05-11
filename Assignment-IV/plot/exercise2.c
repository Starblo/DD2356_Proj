#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

size_t N=1000; // Matrix size

void initialize_matrix(double * matrix) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i * N + j] = i + j * 0.01;
        }
    }
}

void compute_row_sums(double * local_matrix, double * local_row_sums, int rank, int nprocs) {
    int chunk_size = N / nprocs;
    for (int i = 0; i < chunk_size; i++) {
        local_row_sums[i] = 0.0;
        for (int j = 0; j < N; j++) {
            local_row_sums[i] += local_matrix[i * N + j];
        }
    }
}

void write_output(double * row_sums) {
    FILE *f = fopen("row_sums_output.log", "w");
    for (int i = 0; i < N; i++) {
        fprintf(f, "%f\n", row_sums[i]);
    }
    fclose(f);
}

int main(int argc, char** argv) {
    int rank, nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    printf("MPI rank %d out of %d processes.\n", rank, nprocs);

    double start_time = MPI_Wtime();

    double * matrix = NULL;
    double * row_sums = NULL;
    double * local_row_sums = malloc(N / nprocs * sizeof(double));
    double * local_matrix = malloc(N * N / nprocs * sizeof(double));
    if (rank == 0) {
        row_sums = malloc(N * sizeof(double));
        matrix = malloc(N * N * sizeof(double));
        initialize_matrix(matrix);
    }

    MPI_Scatter(matrix, // send buffer
                N * N / nprocs, // send count
                MPI_DOUBLE, // send type
                local_matrix, // receive buffer
                N * N / nprocs, // receive count
                MPI_DOUBLE, // receive type
                0, // root rank
                MPI_COMM_WORLD); // communicator

    compute_row_sums(local_matrix, local_row_sums, rank, nprocs);

    MPI_Gather(local_row_sums, // send buffer
                N / nprocs, // send count
                MPI_DOUBLE, // send type
                row_sums, // receive buffer
                N / nprocs, // receive count
                MPI_DOUBLE, // receive type
                0, // root rank
                MPI_COMM_WORLD); // communicator
    
    if (rank == 0) {
        write_output(row_sums);
    }

    // Compute the sums of the whole matrix
    double global_sum = 0.0;
    double local_sum = 0.0;
    for (int i = 0; i < N / nprocs; i++) {
        local_sum += local_row_sums[i];
    }


    MPI_Reduce(&local_sum, // send buffer
                &global_sum, // receive buffer
                1, // send count
                MPI_DOUBLE, // send type
                MPI_SUM, // operation
                0, // root rank
                MPI_COMM_WORLD); // communicator
    
    printf("Rank %d computed global sum.\n", rank);

    double end_time = MPI_Wtime(); 
    double elapsed = end_time - start_time;
    MPI_Finalize();
    
    if (rank == 0){
        printf("Row sum computation complete.\n");
        printf("Elapsed time: %f seconds\n", elapsed);
        printf("Global sum: %f\n", global_sum);
        free(matrix);
        free(row_sums);
    }
    free(local_row_sums);
    free(local_matrix);


    return 0;
}