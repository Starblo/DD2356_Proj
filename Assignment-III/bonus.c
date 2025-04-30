#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NEURONS 1000
#define STEPS 500
#define THRESHOLD 50.0

double potentials[NEURONS];
int firings[NEURONS];

void simulate()
{
    FILE *f = fopen("neuron_output.txt", "w");
    for (int step = 0; step < STEPS; step++)
    {
        for (int i = 0; i < NEURONS; i++)
        {
            potentials[i] += rand() % 10;
            if (potentials[i] > THRESHOLD)
            {
                firings[i]++;
                potentials[i] = 0; // Reset potential
            }
            fprintf(f, "%d %d %f\n", step, i, potentials[i]);
        }
    }
    fclose(f);
}

int main()
{
    srand(0);
    for (int i = 0; i < NEURONS; i++)
    {
        potentials[i] = rand() % 20;
        firings[i] = 0;
    }
    double start_time = clock();
    simulate();
    double end_time = clock();
    printf("Simulation time: %f seconds\n", (end_time - start_time) / CLOCKS_PER_SEC);

    FILE *fsummary = fopen("neuron_summary.txt", "w");
    for (int i = 0; i < NEURONS; i++)
    {
        fprintf(fsummary, "Neuron %d fired %d times\n", i, firings[i]);
    }
    fclose(fsummary);

    return 0;
}