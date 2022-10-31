#!/bin/bash
#SBATCH --job-name=your_job_name # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=1                   # number of processes = 1 
#SBATCH --cpus-per-task=20      # Number of CPU cores allocated to each process
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010434/ass2/CSC4005/project2_template
./pthread 1000 1000 100 1
./pthread 1000 1000 100 5
./pthread 1000 1000 100 20
./pthread 1000 1000 100 40
./pthread 1000 1000 100 60
./pthread 1000 1000 100 80
./pthread 1000 1000 100 100
./pthread 1000 1000 100 120
./pthread 1000 1000 100 140
./pthread 1000 1000 100 160
./pthread 1000 1000 100 180
./pthread 1000 1000 100 200

# ./pthread 1000 1000 1000 1
# ./pthread 1000 1000 1000 5
# ./pthread 1000 1000 1000 20
# ./pthread 1000 1000 1000 40
# ./pthread 1000 1000 1000 60
# ./pthread 1000 1000 1000 80
# ./pthread 1000 1000 1000 100
# ./pthread 1000 1000 1000 120
# ./pthread 1000 1000 1000 140
# ./pthread 1000 1000 1000 160
# ./pthread 1000 1000 1000 180
# ./pthread 1000 1000 1000 200

# ./pthread 5000 5000 500 1
# ./pthread 5000 5000 500 5
# ./pthread 5000 5000 500 20
# ./pthread 5000 5000 500 40
# ./pthread 5000 5000 500 60
# ./pthread 5000 5000 500 80
# ./pthread 5000 5000 500 100
# ./pthread 5000 5000 500 120
# ./pthread 5000 5000 500 140
# ./pthread 5000 5000 500 160
# ./pthread 5000 5000 500 180
# ./pthread 5000 5000 500 200



