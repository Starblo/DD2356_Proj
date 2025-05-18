#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>
#include <time.h>

#define N 40000  // Matrix size

void initialize(double *matrix, double *vector) {
    for (int i = 0; i < N * N; i++) {
        matrix[i] = (double)(i % 100) / 10.0;
    }
    for (int i = 0; i < N; i++) {
        vector[i] = (double)(i % 50) / 5.0;
    }
}

int main() {
    // int N = 10000;  // Default matrix size
    // if (argc > 1) {
    //     N = atoi(argv[1]);  // Set N from command-line argument if provided
    // }
    double *matrix = (double*) malloc(N * N * sizeof(double));
    double *vector = (double*) malloc(N * sizeof(double));
    double *result = (double*) malloc(N * sizeof(double));
    
    initialize(matrix, vector);
    
    // Measure the time taken for serial computation
    clock_t start_time = clock();
    
    // Perform matrix-vector multiplication using BLAS
    cblas_dgemv(CblasRowMajor, CblasNoTrans, N, N, 1.0, matrix, N, vector, 1, 0.0, result, 1);
    
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Serial computation time: %f seconds\n", elapsed_time);

    // Write output to file
    FILE *f = fopen("blas_output.txt", "w");
    for (int i = 0; i < N; i++) {
        fprintf(f, "%f\n", result[i]);
    }
    fclose(f);
    
    free(matrix);
    free(vector);
    free(result);
    printf("BLAS matrix-vector multiplication complete.\n");
    return 0;
}