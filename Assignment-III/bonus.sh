#!/usr/bin/bash

# Set the allocation to be charged for this job
# not required if you have set a default allocation
#SBATCH -A edu25.DD2356

# The name of the script is myjob
#SBATCH -J bonus3nonode

# The partition
#SBATCH -p shared

# 1 hour wall clock time will be given to this job
#SBATCH -t 00:10:00

# Number of MPI processes
#SBATCH -n 1

#SBATCH --nodes=1

#SBATCH --cpus-per-task=8

#SBATCH --output=%x_%j.out    # Output file format: jobname_jobid.out

# module load  cray-mpich/8.1.28

echo "serial simulation"
srun ./bonus

export OMP_NUM_THREADS=8

echo "testing different neurons"

for N in {500..3000..500}; do
  gcc -fopenmp -DNEURONS=$N -o bonus_omp bonus_omp.c
  echo "=== Running with NEURONS=$N ==="
  srun ./bonus_omp
done

echo "testing different threshold"
for T in {30..100..10}; do
  gcc -fopenmp -DTHRESHOLD=$T -o bonus_omp bonus_omp.c
  echo "=== Running with THRESHOLD=$T ==="
  srun ./bonus_omp
done