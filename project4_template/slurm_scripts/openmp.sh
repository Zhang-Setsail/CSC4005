#!/bin/bash
#SBATCH --job-name=your_job_name # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=1                   # number of processes = 1 
#SBATCH --cpus-per-task=20      # Number of CPU cores allocated to each process
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010434/ass2/CSC4005/project4_template/
./openmp 100 4
./openmp 100 8
./openmp 100 12
./openmp 100 16
./openmp 100 20
./openmp 100 24
./openmp 100 28
./openmp 100 32
./openmp 100 36

./openmp 1000 4
./openmp 1000 8
./openmp 1000 12
./openmp 1000 16
./openmp 1000 20
./openmp 1000 24
./openmp 1000 28
./openmp 1000 32
./openmp 1000 36

./openmp 5000 4
./openmp 5000 8
./openmp 5000 12
./openmp 5000 16
./openmp 5000 20
./openmp 5000 24
./openmp 5000 28
./openmp 5000 32
./openmp 5000 36