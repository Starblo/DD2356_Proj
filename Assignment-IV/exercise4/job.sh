#!/usr/bin/bash 

# Set the allocation to be charged for this job
# not required if you have set a default allocation
#SBATCH -A edu25.DD2356

# The name of the script is myjob
#SBATCH -J E4Q4plot

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


# module load PDC/23.12
# module load openblas/0.3.28-cpeGNU-23.12

# question 1, 2, 3
# mpicc -o blas_mvm blas_example.c -lopenblas -lm
# srun --ntasks=1 ./blas_mvm

# mpicc -o blas_para blas_para.c -lopenblas -lm
# srun -n 8 ./blas_para

# question 4
# Loop over different matrix sizes
# for matrixsize in 1000 10000 20000 40000 50000
# do
#     # Compile and run the serial version (blas_mvm)
#     echo "Running serial version with matrix size $matrixsize"
#     mpicc -o blas_mvm blas_example.c -lopenblas -lm
#     srun --ntasks=1 ./blas_mvm $matrixsize

#     # Compile and run the parallel version (blas_para)
#     echo "Running parallel version with matrix size $matrixsize"
#     mpicc -o blas_para blas_para.c -lopenblas -lm
#     srun -n 16 ./blas_para $matrixsize

#     echo "----------------------------------------"
# done

module load PDC/23.12
module load openblas/0.3.28-cpeGNU-23.12
module load score-p/8.4-cpeGNU

# mpicc -o blas_mvm blas_example.c -lopenblas -lm
# srun --ntasks=1 ./blas_mvm

# scorep mpicc -o blas_para_opt blas_para_optimized.c -lopenblas -lm
# export SCOREP_ENABLE_PROFILING=true
# export SCOREP_ENABLE_TRACING=false
# srun -n 8 ./blas_para_opt
# export SCOREP_ENABLE_PROFILING=false
# export SCOREP_ENABLE_TRACING=true
# srun -n 8 ./blas_para_opt

# Loop over different matrix sizes
for matrixsize in 1000 10000 20000 40000
do
    # Compile and run the serial version (blas_mvm)
    echo "Running serial version with matrix size $matrixsize"
    mpicc -o blas_mvm blas_example.c -lopenblas -lm
    srun --ntasks=1 ./blas_mvm $matrixsize

    echo "----------------------------------------"
done
for matrixsize in 1000 10000 20000 40000 50000 60000 70000 80000
do
    # Compile and run the parallel version (blas_para)
    echo "Running parallel version with matrix size $matrixsize"
    mpicc -o blas_para_opt blas_para_optimized.c -lopenblas -lm
    srun -n 16 ./blas_para_opt $matrixsize

    echo "----------------------------------------"
done