#!/bin/bash
#SBATCH --job-name=your_job_name # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=1                   # number of processes = 1 
#SBATCH --cpus-per-task=20      # Number of CPU cores allocated to each process
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010434/ass2/CSC4005/project4_template/
./pthread 100 4
./pthread 100 8
./pthread 100 12
./pthread 100 16
./pthread 100 20
./pthread 100 24
./pthread 100 28
./pthread 100 32
./pthread 100 36

./pthread 1000 4
./pthread 1000 8
./pthread 1000 12
./pthread 1000 16
./pthread 1000 20
./pthread 1000 24
./pthread 1000 28
./pthread 1000 32
./pthread 1000 36

./pthread 5000 4
./pthread 5000 8
./pthread 5000 12
./pthread 5000 16
./pthread 5000 20
./pthread 5000 24
./pthread 5000 28
./pthread 5000 32
./pthread 5000 36