#!/usr/bin/bash 

# Set the allocation to be charged for this job
# not required if you have set a default allocation
#SBATCH -A edu25.DD2356

# The name of the script is myjob
#SBATCH -J exercise2

# The partition
#SBATCH -p main

# 1 hour wall clock time will be given to this job
#SBATCH -t 00:03:00

# Number of MPI processes
#SBATCH -n 1

#SBATCH --nodes=1

#SBATCH --cpus-per-task=256

#SBATCH --output=%x_%j.out    # Output file format: jobname_jobid.out

module load  cray-mpich/8.1.28

# # Display hardware info
# srun numactl --show
# rm topo.svg
# lstopo topo.svg

# # Compile the code
gcc -O3 -fopenmp -march=native -mcmodel=large -DSTREAM_ARRAY_SIZE=300000000 stream_schedule.c -o stream_schedule


# # Do experiment of different number of threads
# # Compiled executable
EXEC=./stream_schedule
export OMP_SCHEDULE=guided

# Number of threads to test
THREADS=(1 32 64 128)

# Loop over each thread count
for t in "${THREADS[@]}"
do
    echo "Running with $t threads..."
    export OMP_NUM_THREADS=$t

    # Run 5 times for each thread count
    for run in {1..5}
    do
        echo "Run $run with $t threads"
        srun --exclusive --cpu-bind=cores $EXEC
    done
done



# Executable
EXEC=./stream_schedule

# Schedules to test
SCHEDULES=("static" "dynamic" "guided")

# Always use 128 threads
export OMP_NUM_THREADS=128

# Loop over each schedule type
for sched in "${SCHEDULES[@]}"
do
    echo "Running with schedule: $sched"

    # Set OpenMP schedule via environment variable
    export OMP_SCHEDULE=$sched

    # Run 5 times for each schedule
    for run in {1..5}
    do
        echo "Run $run with $sched schedule"
        srun --exclusive --cpu-bind=cores $EXEC
    done
done