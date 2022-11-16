#!/bin/bash

#SBATCH --job-name CSC3150CUDADemo  ## Job name
#SBATCH --gres=gpu:1                ## Number of GPUs required for job execution.
#SBATCH --output result.out         ## filename of the output
#SBATCH --partition=Project           ## the partitions to run in (Debug or Project)
#SBATCH --ntasks=1                  ## number of tasks (analyses) to run
#SBATCH --gpus-per-task=1           ## number of gpus per task
#SBATCH --time=0-00:02:00           ## time for analysis (day-hour:min:sec)

## Compile the cuda script using the nvcc compiler
## You can compile your codes out of the script and simply srun the executable file.
cd /nfsmnt/119010434/ass2/CSC4005/project3_template_new/
## Run the script
./cuda 100 10
./cuda 1000 10
./cuda 10000 10
./cuda 100000 10
# ./cuda 10000 100
# ./cuda 10000 100
