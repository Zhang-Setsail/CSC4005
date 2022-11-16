#!/bin/bash
#SBATCH --job-name=your_job_name # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=1                   # number of processes = 1 
#SBATCH --cpus-per-task=20      # Number of CPU cores allocated to each process
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010434/ass2/CSC4005/project3_template_new/

./openmp 100 10 4
./openmp 1000 10 4
./openmp 10000 10 4

./openmp 100 10 8
./openmp 1000 10 8
./openmp 10000 10 8

./openmp 100 10 12
./openmp 1000 10 12
./openmp 10000 10 12

./openmp 100 10 16
./openmp 1000 10 16
./openmp 10000 10 16

./openmp 100 10 20
./openmp 1000 10 20
./openmp 10000 10 20

./openmp 100 10 24
./openmp 1000 10 24
./openmp 10000 10 24

./openmp 100 10 28
./openmp 1000 10 28
./openmp 10000 10 28

./openmp 100 10 32
./openmp 1000 10 32
./openmp 10000 10 32

./openmp 100 10 36
./openmp 1000 10 36
./openmp 10000 10 36
