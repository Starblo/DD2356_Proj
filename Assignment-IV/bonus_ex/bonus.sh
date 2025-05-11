#!/usr/bin/bash 

# Set the allocation to be charged for this job
# not required if you have set a default allocation
#SBATCH -A edu25.DD2356

# The name of the script is myjob
#SBATCH -J bonus

# The partition
#SBATCH -p shared

# 30 minutes wall clock time will be given to this job
#SBATCH -t 00:30:00

#SBATCH --nodes=1

# Number of MPI processes
#SBATCH --ntasks=32  

#SBATCH --output=%x_%j.out    # Output file format: jobname_jobid.out

# Run the executable named myexe
# and write the output into my output file

mpicc -o bonus bonus.c
cc -o bonus_serial bonus_serial.c

EXE=./bonus

for np in $(seq 2 2 32); do
    echo "Running with $np processes..."
    srun -n $np $EXE
    echo "-------------------------------"
done

srun -n 1 ./bonus_serial