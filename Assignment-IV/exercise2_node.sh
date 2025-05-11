#!/usr/bin/bash 

# Set the allocation to be charged for this job
# not required if you have set a default allocation
#SBATCH -A edu25.DD2356

# The name of the script is myjob
#SBATCH -J exercise2_node

# The partition
#SBATCH -p main

#SBATCH --nodes=8

# 5 minutes of wall clock time
#SBATCH -t 00:05:00

#SBATCH --ntasks-per-node=1

#SBATCH --cpus-per-task=64


#SBATCH --output=%x_%j.out    # Output file format: jobname_jobid.out

# Run the executable named myexe
# and write the output into my output file

mpicc -o exercise2 exercise2.c

EXE=./exercise2

for node in 2 4 5 8; do
    echo "Running with $node nodes..."
    srun --ntasks-per-node=1  -N $node $EXE
    echo "-------------------------------"
done
