#!/bin/bash
#SBATCH --job-name=your_job_name # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=20                   # number of processes = 20
#SBATCH --cpus-per-task=1      # Number of CPU cores allocated to each process (please use 1 here, in comparison with pthread)
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010434/ass2/CSC4005/project4_template/
mpirun -np 2 ./mpi 100
mpirun -np 4 ./mpi 100
mpirun -np 10 ./mpi 100
mpirun -np 20 ./mpi 100
mpirun -np 40 ./mpi 100

mpirun -np 2 ./mpi 1000
mpirun -np 4 ./mpi 1000
mpirun -np 10 ./mpi 1000
mpirun -np 20 ./mpi 1000
mpirun -np 40 ./mpi 1000

mpirun -np 2 ./mpi 5000
mpirun -np 4 ./mpi 5000
mpirun -np 10 ./mpi 5000
mpirun -np 20 ./mpi 5000
mpirun -np 40 ./mpi 5000