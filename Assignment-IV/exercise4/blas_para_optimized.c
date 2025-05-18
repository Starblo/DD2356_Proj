#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>
#include <mpi.h>
#include <string.h>

#define N 80000  // Matrix size

// Function to generate local matrix and vector for each process
void generate_local_matrix(double *local_matrix, int start_row, int num_rows) {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < N; j++) {
            local_matrix[i * N + j] = (double)(((start_row + i) * N + j) % 100) / 10.0;
        }
    }
}

// Function to generate the vector
void generate_vector(double *vector) {
    for (int i = 0; i < N; i++) {
        vector[i] = (double)(i % 50) / 5.0;
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    double start_time, end_time;
    
    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Calculate the number of rows each process will handle
    int rows_per_proc = N / size;
    int my_start_row = rank * rows_per_proc;
    int my_rows = (rank == size - 1) ? (N - my_start_row) : rows_per_proc;
    
    // Synchronize all processes and start the timer
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();
    
    // Allocate memory for vector, local matrix, and result
    double *vector = (double *)malloc(N * sizeof(double));
    double *my_matrix = (double *)malloc(my_rows * N * sizeof(double));
    double *my_result = (double *)malloc(my_rows * sizeof(double));
    
    // Check if memory allocation failed
    if (!vector || !my_matrix || !my_result) {
        fprintf(stderr, "Process %d: Memory allocation failed\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    // Optimization 1: Generate local matrix and vector data on each process to avoid communication for data distribution
    generate_vector(vector);
    generate_local_matrix(my_matrix, my_start_row, my_rows);
    
    // Optimization 2: Direct matrix-vector multiplication
    cblas_dgemv(CblasRowMajor, CblasNoTrans, my_rows, N, 1.0, 
                my_matrix, N, vector, 1, 0.0, my_result, 1);
    
    // Prepare to collect the results
    double *result = NULL;
    if (rank == 0) {
        result = (double *)malloc(N * sizeof(double));
        if (!result) {
            fprintf(stderr, "Process 0: Result memory allocation failed\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    
    // Optimization 3: Use standardized collective operation to gather results
    int *recvcounts = NULL;
    int *displs = NULL;
    
    if (rank == 0) {
        recvcounts = (int *)malloc(size * sizeof(int));
        displs = (int *)malloc(size * sizeof(int));
        
        for (int i = 0; i < size; i++) {
            recvcounts[i] = (i == size - 1) ? (N - i * rows_per_proc) : rows_per_proc;
            displs[i] = i * rows_per_proc;
        }
    }
    
    // Use MPI_Gatherv to gather results from all processes
    MPI_Gatherv(my_result, my_rows, MPI_DOUBLE, 
               result, recvcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // Root process handles the results and outputs them
    if (rank == 0) {
        end_time = MPI_Wtime();
        printf("Zero-communication parallel time with %d processes: %f seconds\n", 
               size, end_time - start_time);
        
        // Write results to file
        FILE *f = fopen("mpi_blas_zero_comm.txt", "w");
        if (f) {
            for (int i = 0; i < N; i++) {
                fprintf(f, "%f\n", result[i]);
            }
            fclose(f);
            printf("Zero-communication MPI BLAS matrix-vector multiplication complete.\n");
        }
        
        // Free allocated memory
        free(recvcounts);
        free(displs);
        free(result);
    }
    
    // Free memory
    free(vector);
    free(my_matrix);
    free(my_result);
    
    // Finalize MPI
    MPI_Finalize();
    return 0;
}
