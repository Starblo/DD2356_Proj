#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 500  // Grid size
#define ITER 1000  // Number of iterations
#define DT 0.1  // Time step
#define DX 1.0   // Grid spacing

double h[N][N], u[N][N], v[N][N];

void initialize() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            h[i][j] = 1.0;
            u[i][j] = 0.0;
            v[i][j] = 0.0;
        }
    int center = N / 2;
    h[center][center] = 10.0;
    h[center-1][center] = 5.0;
    h[center+1][center] = 5.0;
    h[center][center-1] = 5.0;
    h[center][center+1] = 5.0;
}

// void compute() {
//     for (int iter = 0; iter < ITER; iter++) {
//         #pragma omp parallel for collapse(2) schedule(static)
//         for (int i = 1; i < N - 1; i++) {
//             for (int j = 1; j < N - 1; j++) {
//                 double dudx = (u[i+1][j] - u[i-1][j]) / (2.0 * DX);
//                 double dvdy = (v[i][j+1] - v[i][j-1]) / (2.0 * DX);
//                 h[i][j] -= DT * (dudx + dvdy);
//             }
//         }
//     }
// }

void compute() {
    static double h_new[N][N];

    for (int iter = 0; iter < ITER; iter++) {
        // Step 1: update velocity fields u and v
        #pragma omp parallel for collapse(2) schedule(static)
        for (int i = 1; i < N - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                u[i][j] -= DT * (h[i+1][j] - h[i-1][j]) / (2.0 * DX);
                v[i][j] -= DT * (h[i][j+1] - h[i][j-1]) / (2.0 * DX);
            }
        }

        // Step 2: compute new h using updated u and v
        #pragma omp parallel for collapse(2) schedule(static)
        for (int i = 1; i < N - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                double dudx = (u[i+1][j] - u[i-1][j]) / (2.0 * DX);
                double dvdy = (v[i][j+1] - v[i][j-1]) / (2.0 * DX);
                h_new[i][j] = h[i][j] - DT * (dudx + dvdy);
            }
        }

        // Step 3: copy new h values back
        #pragma omp parallel for collapse(2) schedule(static)
        for (int i = 1; i < N - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                h[i][j] = h_new[i][j];
            }
        }
    }
}

void write_output() {
    FILE *f = fopen("output.txt", "w");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(f, "%f ", h[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

int main() {
    // int thread_counts[] = {1, 16, 32, 64, 128};
    int thread_counts[] = {64};
    int num_configs = sizeof(thread_counts) / sizeof(thread_counts[0]);

    for (int i = 0; i < num_configs; i++) {
        int threads = thread_counts[i];
        omp_set_num_threads(threads);
        initialize();

        double start = omp_get_wtime();
        compute();
        double end = omp_get_wtime();

        printf("Threads: %3d | Execution time: %f seconds\n", threads, end - start);
    }

    write_output();
    printf("Computation completed.\n");
    return 0;
}
