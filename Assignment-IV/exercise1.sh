#!/usr/bin/bash 

# Set the allocation to be charged for this job
# not required if you have set a default allocation
#SBATCH -A edu25.DD2356

# The name of the script is myjob
#SBATCH -J exercise1

# The partition
#SBATCH -p shared

#SBATCH --nodes=1

# 1 hour wall clock time will be given to this job
#SBATCH -t 01:00:00

# Number of MPI processes
#SBATCH --ntasks=32  

#SBATCH --output=%x_%j.out    # Output file format: jobname_jobid.out

# Run the executable named myexe
# and write the output into my output file

module load cce/16.0.1
module load cray-mpich/8.1.27

mpicc -o exercise1 exercise1.c
mpicc -o serial1 exercise1_serial.c

EXE=./exercise1

for np in $(seq 2 2 32); do
    echo "Running with $np processes..."
    srun -n $np $EXE
    echo "-------------------------------"
done

srun -n 1 ./serial1