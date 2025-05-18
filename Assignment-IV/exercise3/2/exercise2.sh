#!/usr/bin/bash 

# Set the allocation to be charged for this job
# not required if you have set a default allocation
#SBATCH -A edu25.DD2356

# The name of the script is myjob
#SBATCH -J exercise2

# The partition
#SBATCH -p shared

# 20 minutes wall clock time will be given to this job
#SBATCH -t 01:00:00

#SBATCH --nodes=1

# Number of MPI processes
#SBATCH --ntasks=32  

#SBATCH --output=%x_%j.out    # Output file format: jobname_jobid.out

# Run the executable named myexe
# and write the output into my output file


module load PDC/23.12
module load score-p/8.4-cpeGNU

scorep mpicc -o exercise2_scorep exercise2.c

export SCOREP_ENABLE_PROFILING=true
export SCOREP_ENABLE_TRACING=false
srun -n 16 ./exercise2_scorep

export SCOREP_ENABLE_PROFILING=false
export SCOREP_ENABLE_TRACING=true
srun -n 16 ./exercise2_scorep