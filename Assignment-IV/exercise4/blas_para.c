#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>
#include <time.h>
#include <mpi.h>

#define N 40000  // Matrix size

void initialize(double *matrix, double *vector) {
    for (int i = 0; i < N * N; i++) {
        matrix[i] = (double)(i % 100) / 10.0;
    }
    for (int i = 0; i < N; i++) {
        vector[i] = (double)(i % 50) / 5.0;
    }
}

int main(int argc, char *argv[]) {
    // int N = 10000;  // Default matrix size
    // if (argc > 1) {
    //     N = atoi(argv[1]);  // Set N from command-line argument if provided
    // }
    int rank, size;
    double start_time, end_time;
    
    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Calculate local portion size
    int rows_per_proc = N / size;
    int local_rows = (rank == size - 1) ? (N - rank * rows_per_proc) : rows_per_proc;
    
    // Allocate memory for the full vector and local matrix portion
    double *vector = (double*) malloc(N * sizeof(double));
    double *local_matrix = (double*) malloc(local_rows * N * sizeof(double));
    double *local_result = (double*) malloc(local_rows * sizeof(double));
    double *result = NULL;
    
    if (rank == 0) {
        // Only root process allocates the full matrix and result
        double *matrix = (double*) malloc(N * N * sizeof(double));
        result = (double*) malloc(N * sizeof(double));
        
        // Initialize data on root process
        initialize(matrix, vector);
        
        // Start timing
        start_time = MPI_Wtime();
        
        // Distribute matrix rows to all processes
        for (int i = 0; i < size; i++) {
            int rows = (i == size - 1) ? (N - i * rows_per_proc) : rows_per_proc;
            if (i == 0) {
                // Copy data for process 0 locally
                for (int j = 0; j < rows * N; j++) {
                    local_matrix[j] = matrix[j];
                }
            } else {
                // Send data to other processes
                MPI_Send(&matrix[i * rows_per_proc * N], rows * N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }
        }
        
        free(matrix);
    } else {
        // Receive matrix portion from root
        MPI_Recv(local_matrix, local_rows * N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    
    // Broadcast the vector to all processes
    MPI_Bcast(vector, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // Each process performs matrix-vector multiplication on its portion
    cblas_dgemv(CblasRowMajor, CblasNoTrans, local_rows, N, 1.0, local_matrix, N, vector, 1, 0.0, local_result, 1);
    
    // Gather results back to root process
    if (rank == 0) {
        // Copy local result to the appropriate position in the full result
        for (int i = 0; i < local_rows; i++) {
            result[i] = local_result[i];
        }
        
        // Receive results from other processes
        for (int i = 1; i < size; i++) {
            int rows = (i == size - 1) ? (N - i * rows_per_proc) : rows_per_proc;
            MPI_Recv(&result[i * rows_per_proc], rows, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        
        // End timing
        end_time = MPI_Wtime();
        printf("Parallel computation time with %d processes: %f seconds\n", size, end_time - start_time);
        
        // Write output to file
        FILE *f = fopen("mpi_blas_output.txt", "w");
        for (int i = 0; i < N; i++) {
            fprintf(f, "%f\n", result[i]);
        }
        fclose(f);
        
        printf("MPI BLAS matrix-vector multiplication complete.\n");
        free(result);
    } else {
        // Send local results back to root
        MPI_Send(local_result, local_rows, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }
    
    // Clean up
    free(vector);
    free(local_matrix);
    free(local_result);
    
    MPI_Finalize();
    return 0;
}
