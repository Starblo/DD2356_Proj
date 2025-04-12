#!/usr/bin/bash

# Set the allocation to be charged for this job
# not required if you have set a default allocation
#SBATCH -A edu25.DD2356

# The name of the script is myjob
#SBATCH -J exercise1

# The partition
#SBATCH -p shared

# 10 minutes wall clock time will be given to this job
#SBATCH -t 00:10:00

# Number of MPI processes
#SBATCH -n 1

# Run the executable named myexe
# and write the output into my output file


perf stat -e cycles,instructions,cache-misses,L1-dcache-loads,L1-dcache-load-misses ./transpose.out
perf stat -e cycles,instructions,cache-misses,L1-dcache-loads,L1-dcache-load-misses ./transpose_opt16.out
perf stat -e cycles,instructions,cache-misses,L1-dcache-loads,L1-dcache-load-misses ./transpose_opt32.out
perf stat -e cycles,instructions,cache-misses,L1-dcache-loads,L1-dcache-load-misses ./transpose_opt64.out
perf stat -e cycles,instructions,cache-misses,L1-dcache-loads,L1-dcache-load-misses ./transpose_opt128.out
perf stat -e cycles,instructions,cache-misses,L1-dcache-loads,L1-dcache-load-misses ./transpose_opt256.out



