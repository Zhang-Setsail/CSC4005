#!/bin/bash
#SBATCH --job-name=your_job_name # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=1                   # number of processes = 1 
#SBATCH --cpus-per-task=20      # Number of CPU cores allocated to each process
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010434/ass2/CSC4005/project3_template_new/

./pthread 100 10 4
./pthread 1000 10 4
./pthread 10000 10 4

./pthread 100 10 8
./pthread 1000 10 8
./pthread 10000 10 8

./pthread 100 10 12
./pthread 1000 10 12
./pthread 10000 10 12

./pthread 100 10 16
./pthread 1000 10 16
./pthread 10000 10 16

./pthread 100 10 20
./pthread 1000 10 20
./pthread 10000 10 20

./pthread 100 10 24
./pthread 1000 10 24
./pthread 10000 10 24

./pthread 100 10 28
./pthread 1000 10 28
./pthread 10000 10 28

./pthread 100 10 32
./pthread 1000 10 32
./pthread 10000 10 32

./pthread 100 10 36
./pthread 1000 10 36
./pthread 10000 10 36
