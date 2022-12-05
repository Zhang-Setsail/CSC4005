#!/bin/bash
#SBATCH --job-name=your_job_name # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=1                   # number of processes = 1 
#SBATCH --cpus-per-task=20      # Number of CPU cores allocated to each process
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010434/ass2/CSC4005/project4_template/
./openmp 1000 4
./openmp 1000 20
./openmp 1000 40
./openmp 1000 80
./openmp 1000 120
./openmp 1000 200