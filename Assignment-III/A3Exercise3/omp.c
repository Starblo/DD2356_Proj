#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define MAX_RUNS 10

typedef struct {
    double val;
    char pad[64];
} padded_sum_t;

double serial_sum(double *x, size_t size) {
    double sum_val = 0.0;
    for (size_t i = 0; i < size; i++) {
        sum_val += x[i];
    }
    return sum_val;
}

double omp_sum(double *x, size_t size) {
    double sum_val = 0.0;
    #pragma omp parallel for
    for (size_t i = 0; i < size; i++) {
        sum_val += x[i];
    }
    return sum_val;
}

double omp_critical_sum(double *x, size_t size) {
    double sum_val = 0.0;
    #pragma omp parallel for
    for (size_t i = 0; i < size; i++) {
        #pragma omp critical
        sum_val += x[i];
    }
    return sum_val;
}

double omp_local_sum(double *x, size_t size) {
    double sum_val = 0.0;
    double *local_sum = NULL;
    int actual_num_threads = 0;
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        #pragma omp single
        {
            actual_num_threads = num_threads;
            local_sum = (double*)malloc(num_threads * sizeof(double));
        }
        size_t chunk_size = size / num_threads;
        size_t start_idx = tid * chunk_size;
        size_t end_idx = (tid == num_threads - 1) ? size : start_idx + chunk_size;
        local_sum[tid] = 0.0;
        for (size_t i = start_idx; i < end_idx; ++i) {
            local_sum[tid] += x[i];
        }
    }
    for (int i = 0; i < actual_num_threads; ++i) {
        sum_val += local_sum[i];
    }
    free(local_sum);
    return sum_val;
}

double omp_localpad_sum(double *x, size_t size) {
    double sum_val = 0.0;
    padded_sum_t *local_sums = NULL;
    int actual_num_threads = 0;
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        #pragma omp single
        {
            actual_num_threads = num_threads;
            local_sums = (padded_sum_t*)malloc(num_threads * sizeof(padded_sum_t));
            for (int i = 0; i < num_threads; i++) {
                local_sums[i].val = 0.0;
            }
        }
        size_t chunk_size = size / num_threads;
        size_t start = tid * chunk_size;
        size_t end = (tid == num_threads - 1) ? size : start + chunk_size;
        for (size_t i = start; i < end; i++) {
            local_sums[tid].val += x[i];
        }
    }
    for (int i = 0; i < actual_num_threads; i++) {
        sum_val += local_sums[i].val;
    }
    free(local_sums);
    return sum_val;
}

void generate_random(double *input, size_t size) {
    for (size_t i = 0; i < size; i++) {
        input[i] = rand() / (double)(RAND_MAX);
    }
}

int main() {
    size_t size = 10000000; // 10^7
    double *array = malloc(size * sizeof(double));
    generate_random(array, size);

    // int thread_counts[] = {1, 2, 4, 8, 16, 20, 24, 28, 32, 64, 128};
    int thread_counts[] = {1, 32, 64, 128};
    int num_configs = sizeof(thread_counts) / sizeof(thread_counts[0]);
    double serialsum = serial_sum(array, size);
    printf("serial sum: %f\n", serialsum);

    for (int c = 0; c < num_configs; c++) {
        int threads = thread_counts[c];
        omp_set_num_threads(threads);

        double times[MAX_RUNS];
        for (int i = 0; i < MAX_RUNS; i++) {
            double start = omp_get_wtime();
            // double sum = omp_sum(array, size);
            // double sum = omp_critical_sum(array, size);
            // double sum = omp_local_sum(array, size);
            double sum = omp_localpad_sum(array, size);
            double end = omp_get_wtime();
            times[i] = end - start;
            printf("result: %f\n", sum);
        }

        double total = 0.0;
        for (int i = 0; i < MAX_RUNS; i++) total += times[i];
        double avg = total / MAX_RUNS;

        double var = 0.0;
        for (int i = 0; i < MAX_RUNS; i++) var += (times[i] - avg) * (times[i] - avg);
        var /= MAX_RUNS;
        double stddev = sqrt(var);

        printf("Threads: %2d | Avg time: %f s | Stddev: %f s\n", threads, avg, stddev);
    }

    free(array);
    return 0;
}
