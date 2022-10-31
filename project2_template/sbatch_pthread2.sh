#!/bin/bash
#SBATCH --job-name=your_job_name # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=1                   # number of processes = 1 
#SBATCH --cpus-per-task=20      # Number of CPU cores allocated to each process
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010434/ass2/CSC4005/project2_template
./pthread 1000 1000 100 1
./pthread 1000 1000 100 2
./pthread 1000 1000 100 3
./pthread 1000 1000 100 4
./pthread 1000 1000 100 5
./pthread 1000 1000 100 6
./pthread 1000 1000 100 7
./pthread 1000 1000 100 8
./pthread 1000 1000 100 9
./pthread 1000 1000 100 10
./pthread 1000 1000 100 11
./pthread 1000 1000 100 12
./pthread 1000 1000 100 13
./pthread 1000 1000 100 14
./pthread 1000 1000 100 15
./pthread 1000 1000 100 16
./pthread 1000 1000 100 17
./pthread 1000 1000 100 18
./pthread 1000 1000 100 19
./pthread 1000 1000 100 20

# ./pthread 1000 1000 1000 1
# ./pthread 1000 1000 1000 2
# ./pthread 1000 1000 1000 3
# ./pthread 1000 1000 1000 4
# ./pthread 1000 1000 1000 5
# ./pthread 1000 1000 1000 6
# ./pthread 1000 1000 1000 7
# ./pthread 1000 1000 1000 8
# ./pthread 1000 1000 1000 9
# ./pthread 1000 1000 1000 10
# ./pthread 1000 1000 1000 11
# ./pthread 1000 1000 1000 12
# ./pthread 1000 1000 1000 13
# ./pthread 1000 1000 1000 14
# ./pthread 1000 1000 1000 15
# ./pthread 1000 1000 1000 16
# ./pthread 1000 1000 1000 17
# ./pthread 1000 1000 1000 18
# ./pthread 1000 1000 1000 19
# ./pthread 1000 1000 1000 20

# ./pthread 5000 5000 500 1
# ./pthread 5000 5000 500 2
# ./pthread 5000 5000 500 3
# ./pthread 5000 5000 500 4
# ./pthread 5000 5000 500 5
# ./pthread 5000 5000 500 6
# ./pthread 5000 5000 500 7
# ./pthread 5000 5000 500 8
# ./pthread 5000 5000 500 9
# ./pthread 5000 5000 500 10
# ./pthread 5000 5000 500 11
# ./pthread 5000 5000 500 12
# ./pthread 5000 5000 500 13
# ./pthread 5000 5000 500 14
# ./pthread 5000 5000 500 15
# ./pthread 5000 5000 500 16
# ./pthread 5000 5000 500 17
# ./pthread 5000 5000 500 18
# ./pthread 5000 5000 500 19
# ./pthread 5000 5000 500 20



