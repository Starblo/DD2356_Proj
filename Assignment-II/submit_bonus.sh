#!/usr/bin/bash 

# Set the allocation to be charged for this job
# not required if you have set a default allocation
#SBATCH -A edu25.DD2356

# The name of the script is myjob
#SBATCH -J Bonus

# The partition
#SBATCH -p shared

# 30 minutes wall clock time will be given to this job
#SBATCH -t 00:30:00

# Number of MPI processes

#SBATCH --nodes=2

#SBATCH --ntasks-per-node=1

#SBATCH --output=%x_%j.out    # Output file format: jobname_jobid.out

# Run the executable named myexe
# and write the output into my output file
# module load  cray-mpich/8.1.28

srun -N 2 -n 2 ./mpi_ping_pong