#!/usr/bin/bash 

# Set the allocation to be charged for this job
# not required if you have set a default allocation
#SBATCH -A edu25.DD2356

# The name of the script is myjob
#SBATCH -J exercise2

# The partition
#SBATCH -p shared

# 20 minutes wall clock time will be given to this job
#SBATCH -t 00:20:00

#SBATCH --nodes=1

# Number of MPI processes
#SBATCH --ntasks=32  

#SBATCH --output=%x_%j.out    # Output file format: jobname_jobid.out

# Run the executable named myexe
# and write the output into my output file

mpicc -o exercise2 exercise2.c
cc -o serial2 exercise2_serial.c

EXE=./exercise2

for np in 2 4 5 8 10 20 25 32; do
    echo "Running with $np processes..."
    srun -n $np $EXE
    echo "-------------------------------"
done

srun -n 1 ./serial2