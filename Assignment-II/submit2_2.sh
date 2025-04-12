#!/usr/bin/bash 

# Set the allocation to be charged for this job
# not required if you have set a default allocation
#SBATCH -A edu25.DD2356

# The name of the script is myjob
#SBATCH -J exercise2

# The partition
#SBATCH -p main

# 10 minutes wall clock time will be given to this job
#SBATCH -t 00:10:00

# Number of MPI processes
#SBATCH -n 1

#SBATCH --nodes=1

#SBATCH --cpus-per-task=256

#SBATCH --output=%x_%j.out    # Output file format: jobname_jobid.out

# Run the executable named myexe
# and write the output into my output file
# module load  cray-mpich/8.1.28

srun --ntasks=1 --cpus-per-task=256 ./exercise2
lscpu
rm topo.png topo.svg
lstopo topo.svg
export OMP_NUM_THREADS=256
srun --ntasks=1 --cpus-per-task=256 ./stream_omp