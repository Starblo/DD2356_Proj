#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define STEPS 100  // Simulation steps

size_t N = 2000;  // Grid size

int * grid;
int * new_grid;

void initialize(int start, int end, int rank) {
    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            grid[i * N + j] = rand() % 2;  // Random initial state
        }
    }
}

int count_neighbors(int x, int y) {
    int sum = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int nx = (x + i + N) % N;
            int ny = (y + j + N) % N;
            sum += grid[nx * N + ny];
        }
    }
    return sum;
}

void update(int start, int end, int rank, int nprocs, int step) {
    MPI_Request send_request[2], recv_request[2];
    MPI_Status status[2];
    int prev_rank = (rank - 1 + nprocs) % nprocs;
    int next_rank = (rank + 1) % nprocs;
    
    int recv_indices[2] = {(start + N - 1) % N, end % N};

    // send the first row to the previous rank
    MPI_Isend(&grid[start * N], // send buffer
               N, // size of the data
              MPI_INT, // data type
              prev_rank, // destination rank
              step + N, // tag
              MPI_COMM_WORLD, // communicator
              &send_request[0]); // request

    // send the last row to the next rank
    MPI_Isend(&grid[(end - 1) * N], // send buffer
               N, // size of the data
              MPI_INT, // data type
              next_rank, // destination rank
              step, // tag
              MPI_COMM_WORLD, // communicator
              &send_request[1]); // request

    // receive the first row from the previous rank
    MPI_Irecv(&grid[recv_indices[0] * N], // receive buffer
               N, // size of the data
              MPI_INT, // data type
              prev_rank, // source rank
              step, // tag
              MPI_COMM_WORLD, // communicator
              &recv_request[0]); // request

    // receive the last row from the next rank
    MPI_Irecv(&grid[recv_indices[1] * N], // receive buffer
               N, // size of the data
              MPI_INT, // data type
              next_rank, // source rank
              step + N, // tag
              MPI_COMM_WORLD, // communicator
              &recv_request[1]); // request


    for (int i = start + 1; i < end - 1; i++) {
        for (int j = 0; j < N; j++) {
            int neighbors = count_neighbors(i, j);
            if (grid[i * N + j] == 1 && (neighbors < 2 || neighbors > 3)) {
                new_grid[i * N + j] = 0;
            } else if (grid[i * N + j] == 0 && neighbors == 3) {
                new_grid[i * N + j] = 1;
            } else {
                new_grid[i * N + j] = grid[i * N + j];
            }
        }
    }
    MPI_Waitall(2, recv_request, status);

    // if (rank == 1) {
    //     printf("recv indices: %d %d\n", recv_indices[0], recv_indices[1]);
    //     printf("start: %d, end: %d\n", start, end);
    // }
    


    for (int j = 0; j < N; j++) {
        int neighbors = count_neighbors(start, j);
        if (grid[start * N + j] == 1 && (neighbors < 2 || neighbors > 3)) {
            new_grid[start * N + j] = 0;
        } else if (grid[start * N + j] == 0 && neighbors == 3) {
            new_grid[start * N + j] = 1;
        } else {
            new_grid[start * N + j] = grid[start * N + j];
        }
    }

    for (int j = 0; j < N; j++) {
        int neighbors = count_neighbors(end - 1, j);
        if (grid[(end - 1) * N + j] == 1 && (neighbors < 2 || neighbors > 3)) {
            new_grid[(end - 1) * N + j] = 0;
        } else if (grid[(end - 1) * N + j] == 0 && neighbors == 3) {
            new_grid[(end - 1) * N + j] = 1;
        } else {
            new_grid[(end - 1) * N + j] = grid[(end - 1) * N + j];
        }
    }

    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            grid[i * N  + j] = new_grid[i * N + j];
        }
    }

    // print grid in update function
    // if (rank == 1){
    //     printf("after updating grid\n");
    //     printf("Rank %d, Step %d:\n", rank, step);
    //     for (int i = start; i < end; i++) {
    //         for (int j = 0; j < N; j++) {
    //             printf("%d ", grid[i * N + j]);
    //         }
    //         printf("\n");
    //     }
    // }

    MPI_Waitall(2, send_request, status);
}

void write_output(int step, int start, int end, int rank, int nprocs) {
    int recvcounts[nprocs];
    int displs[nprocs]; // displacement array
    int chunk_size = N / nprocs * N;
    for (int i = 0; i < nprocs; i++) {
        recvcounts[i] = chunk_size;
        displs[i] = i * chunk_size;
    }
    recvcounts[nprocs - 1] = N * N - (nprocs - 1) * chunk_size;

    // if(rank == 0) {
    //     printf("displacements: ");
    //     for (int i = 0; i < nprocs; i++) {
    //         printf("%d ", displs[i]);
    //     }
    //     printf("\n");
    // }
    // if(rank == 0) {
    //     printf("recvcounts: ");
    //     for (int i = 0; i < nprocs; i++) {
    //         printf("%d ", recvcounts[i]);
    //     }
    //     printf("\n");
    // }


    int * grid_all = NULL;
    if (rank == 0) {
        grid_all = (int *)malloc(N * N * sizeof(int));
    }

    MPI_Gatherv(&grid[start * N], // send buffer
                (end - start) * N, // send count
                MPI_INT, // send type
                grid_all, // receive buffer
                recvcounts, // receive counts
                displs, // displacements
                MPI_INT, // receive type
                0, // root rank
                MPI_COMM_WORLD); // communicator

    if (rank != 0) return; // Only root process writes output


    char filename[50];
    sprintf(filename, "gol_output_%d.txt", step);
    FILE *f = fopen(filename, "w");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(f, "%d ", grid_all[i * N + j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    free(grid_all);
}

int main(int argc, char** argv) {
    int rank, nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    printf("MPI rank %d out of %d processes.\n", rank, nprocs);

    grid = (int *)malloc(N * N * sizeof(int));
    new_grid = (int *)malloc(N * N * sizeof(int));
    if (grid == NULL || new_grid == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    double start_time = MPI_Wtime();
    srand(rank);

    int start = rank * (N / nprocs);
    int end = (rank + 1) * (N / nprocs);
    end = (rank == nprocs - 1) ? N : end;

    initialize(start, end, rank);

    for (int step = 0; step < STEPS; step++) {
        update(start, end, rank, nprocs, step);
        if (step % 10 == 0) write_output(step, start, end, rank, nprocs);
    }
    
    double end_time = MPI_Wtime(); 
    double elapsed = end_time - start_time;

    MPI_Finalize();
    if (rank == 0) {
        printf("Simulation complete.\n");
        printf("Elapsed time: %f seconds\n", elapsed);
    }
    free(grid);
    free(new_grid);

    return 0;
}