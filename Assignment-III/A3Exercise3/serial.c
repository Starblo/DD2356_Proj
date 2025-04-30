#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

double serial_sum(double *x, size_t size) {
    double sum_val = 0.0;
    for (size_t i = 0; i < size; i++) {
        sum_val += x[i];
    }
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

    const int runs = 10;
    double times[runs];

    for (int i = 0; i < runs; i++) {
        double start = omp_get_wtime();
        double sum = serial_sum(array, size);
        double end = omp_get_wtime();
        times[i] = end - start;
    }

    // 计算平均值和标准差
    double total = 0.0;
    for (int i = 0; i < runs; i++) total += times[i];
    double average = total / runs;

    double variance = 0.0;
    for (int i = 0; i < runs; i++) variance += (times[i] - average) * (times[i] - average);
    variance /= runs;
    double stddev = sqrt(variance);

    printf("Serial sum timing over %d runs:\n", runs);
    for (int i = 0; i < runs; i++) printf("Run %d: %f seconds\n", i+1, times[i]);
    printf("Average time: %f seconds\n", average);
    printf("Standard deviation: %f seconds\n", stddev);

    free(array);
    return 0;
}
