#include <stdio.h>
#include <stdlib.h>
#include <time.h>

size_t N=1500;// Matrix size

void initialize_matrix(double * matrix) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i * N + j] = i + j * 0.01;
        }
    }
}


void compute_row_sums(double * matrix, double * row_sums) {
    for (int i = 0; i < N; i++) {
        row_sums[i] = 0.0;
        for (int j = 0; j < N; j++) {
            row_sums[i] += matrix[i * N + j];
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

int main() {
    clock_t start_time = clock();
    double *matrix = malloc(N * N * sizeof(double));
    double * row_sums = malloc(N * sizeof(double));
    initialize_matrix(matrix);
    compute_row_sums(matrix, row_sums);
    write_output(row_sums);
    double sum = 0.0;
    for (int i = 0; i < N; i++) {
        sum += row_sums[i];
    }
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Elapsed time: %f seconds\n", elapsed_time);
    printf("Total sum of row sums: %f\n", sum);
    printf("Row sum computation complete.\n");
    free(matrix);
    free(row_sums);
    return 0;
}