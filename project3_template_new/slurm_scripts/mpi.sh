#!/bin/bash
#SBATCH --job-name=your_job_name # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=20                   # number of processes = 20
#SBATCH --cpus-per-task=1      # Number of CPU cores allocated to each process (please use 1 here, in comparison with pthread)
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010355/CSC4005_2022Fall_Demo/project3_template/
mpirun -np 4 ./mpi 100 10
mpirun -np 4 ./mpi 1000 10
mpirun -np 4 ./mpi 10000 10

mpirun -np 8 ./mpi 100 10
mpirun -np 8 ./mpi 1000 10
mpirun -np 8 ./mpi 10000 10

mpirun -np 12 ./mpi 100 10
mpirun -np 12 ./mpi 1000 10
mpirun -np 12 ./mpi 10000 10

mpirun -np 16 ./mpi 100 10
mpirun -np 16 ./mpi 1000 10
mpirun -np 16 ./mpi 10000 10

mpirun -np 20 ./mpi 100 10
mpirun -np 20 ./mpi 1000 10
mpirun -np 20 ./mpi 10000 10

mpirun -np 24 ./mpi 100 10
mpirun -np 24 ./mpi 1000 10
mpirun -np 24 ./mpi 10000 10

mpirun -np 28 ./mpi 100 10
mpirun -np 28 ./mpi 1000 10
mpirun -np 28 ./mpi 10000 10

mpirun -np 32 ./mpi 100 10
mpirun -np 32 ./mpi 1000 10
mpirun -np 32 ./mpi 10000 10

mpirun -np 36 ./mpi 100 10
mpirun -np 36 ./mpi 1000 10
mpirun -np 36 ./mpi 10000 10