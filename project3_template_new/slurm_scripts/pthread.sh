#!/bin/bash
#SBATCH --job-name=your_job_name # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=1                   # number of processes = 1 
#SBATCH --cpus-per-task=20      # Number of CPU cores allocated to each process
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010434/ass2/CSC4005/project3_template_new/
# ./pthread 100 100 4
./pthread 1000 1000 40
# ./pthread 1000 100 20
# ./pthread 1000 100 40
# ./pthread 1000 100 80
# ./pthread 1000 100 120
# ./pthread 1000 100 200