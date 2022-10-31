#!/bin/bash
#SBATCH --job-name=your_job_name # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=20                   # number of processes = 20
#SBATCH --cpus-per-task=1      # Number of CPU cores allocated to each process (please use 1 here, in comparison with pthread)
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010434/ass2/CSC4005/project2_template
ulimit -s 1024000000
mpirun -np 1 ./mpi 1000 1000 100
mpirun -np 2 ./mpi 1000 1000 100
mpirun -np 3 ./mpi 1000 1000 100
mpirun -np 4 ./mpi 1000 1000 100
mpirun -np 5 ./mpi 1000 1000 100
mpirun -np 6 ./mpi 1000 1000 100
mpirun -np 7 ./mpi 1000 1000 100
mpirun -np 8 ./mpi 1000 1000 100
mpirun -np 9 ./mpi 1000 1000 100
mpirun -np 10 ./mpi 1000 1000 100
mpirun -np 11 ./mpi 1000 1000 100
mpirun -np 12 ./mpi 1000 1000 100
mpirun -np 13 ./mpi 1000 1000 100
mpirun -np 14 ./mpi 1000 1000 100
mpirun -np 15 ./mpi 1000 1000 100
mpirun -np 16 ./mpi 1000 1000 100
mpirun -np 17 ./mpi 1000 1000 100
mpirun -np 18 ./mpi 1000 1000 100
mpirun -np 19 ./mpi 1000 1000 100
mpirun -np 20 ./mpi 1000 1000 100
# mpirun -np 40 ./mpi 1000 1000 100
# mpirun -np 1 ./mpi 1000 1000 1000
# mpirun -np 2 ./mpi 1000 1000 1000
# mpirun -np 3 ./mpi 1000 1000 1000
# mpirun -np 4 ./mpi 1000 1000 1000
# mpirun -np 5 ./mpi 1000 1000 1000
# mpirun -np 6 ./mpi 1000 1000 1000
# mpirun -np 7 ./mpi 1000 1000 1000
# mpirun -np 8 ./mpi 1000 1000 1000
# mpirun -np 9 ./mpi 1000 1000 1000
# mpirun -np 10 ./mpi 1000 1000 1000
# mpirun -np 11 ./mpi 1000 1000 1000
# mpirun -np 12 ./mpi 1000 1000 1000
# mpirun -np 13 ./mpi 1000 1000 1000
# mpirun -np 14 ./mpi 1000 1000 1000
# mpirun -np 15 ./mpi 1000 1000 1000
# mpirun -np 16 ./mpi 1000 1000 1000
# mpirun -np 17 ./mpi 1000 1000 1000
# mpirun -np 18 ./mpi 1000 1000 1000
# mpirun -np 19 ./mpi 1000 1000 1000
# mpirun -np 20 ./mpi 1000 1000 1000
# mpirun -np 1 ./mpi 5000 5000 500
# mpirun -np 2 ./mpi 5000 5000 500
# mpirun -np 3 ./mpi 5000 5000 500
# mpirun -np 4 ./mpi 5000 5000 500
# mpirun -np 5 ./mpi 5000 5000 500
# mpirun -np 6 ./mpi 5000 5000 500
# mpirun -np 7 ./mpi 5000 5000 500
# mpirun -np 8 ./mpi 5000 5000 500
# mpirun -np 9 ./mpi 5000 5000 500
# mpirun -np 10 ./mpi 5000 5000 500
# mpirun -np 11 ./mpi 5000 5000 500
# mpirun -np 12 ./mpi 5000 5000 500
# mpirun -np 13 ./mpi 5000 5000 500
# mpirun -np 14 ./mpi 5000 5000 500
# mpirun -np 15 ./mpi 5000 5000 500
# mpirun -np 16 ./mpi 5000 5000 500
# mpirun -np 17 ./mpi 5000 5000 500
# mpirun -np 18 ./mpi 5000 5000 500
# mpirun -np 19 ./mpi 5000 5000 500
# mpirun -np 20 ./mpi 5000 5000 500