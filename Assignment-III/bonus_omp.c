#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#ifndef NEURONS
    #define NEURONS 1000
#endif

#define STEPS 500
#ifndef THRESHOLD
    #define THRESHOLD 50.0
#endif

double potentials[NEURONS];
int firings[NEURONS];

void simulate() {
    FILE *f = fopen("neuron_output.txt", "w");
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int step = 0; step < STEPS; step++) {
                for (int i = 0; i < NEURONS; i++) {
                    // Create a task for each neuron update
                    #pragma omp task firstprivate(i, step) shared(potentials, firings, f)
                    {
                        double potential = potentials[i];
                        unsigned int seed = (unsigned int)omp_get_thread_num() + step + i; // Unique seed for each task
                        potential += rand_r(&seed) % 10;
                        
                        if (potential > THRESHOLD) {
                            firings[i]++;  
                            potential = 0.0;
                        }

                        potentials[i] = potential;
                        
                    }
                }
                #pragma omp taskwait
              
                for (int i = 0; i < NEURONS; i++) {
                    fprintf(f, "%d %d %f\n", step, i, potentials[i]);
                }
                
            
            }
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
    double start_time = omp_get_wtime(); 
    simulate();
    double end_time = omp_get_wtime();
    printf("Simulation time: %f seconds\n", end_time - start_time);

    FILE *fsummary = fopen("neuron_summary.txt", "w");
    for (int i = 0; i < NEURONS; i++)
    {
        fprintf(fsummary, "Neuron %d fired %d times\n", i, firings[i]);
    }
    fclose(fsummary);

    return 0;
}