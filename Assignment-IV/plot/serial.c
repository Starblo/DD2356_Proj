#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000 // Matrix size

void initialize_matrix(double matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = i + j * 0.01;
        }
    }
}

void compute_row_sums(double matrix[N][N], double row_sums[N]) {
    for (int i = 0; i < N; i++) {
        row_sums[i] = 0.0;
        for (int j = 0; j < N; j++) {
            row_sums[i] += matrix[i][j];
        }
    }
}

void write_output(double row_sums[N]) {
    FILE *f = fopen("row_sums_output.log", "w");
    for (int i = 0; i < N; i++) {
        fprintf(f, "%f\n", row_sums[i]);
    }
    fclose(f);
}

int main() {
    double matrix[N][N], row_sums[N];
    initialize_matrix(matrix);
    compute_row_sums(matrix, row_sums);
    write_output(row_sums);
    double sum = 0.0;
    for (int i = 0; i < N; i++) {
        sum += row_sums[i];
    }
    printf("Total sum of row sums: %f\n", sum);
    printf("Row sum computation complete.\n");
    return 0;
}