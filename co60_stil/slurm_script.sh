#!/bin/sh
#SBATCH -e my_stderr_%a.txt         # filename for standard error
#SBATCH -o my_stdout_%a.txt         # filename for standard output
#SBATCH --job-name=geant_pTherapy   # name of job shown in `squeue`
#SBATCH --nodelist=node003          # what nodes to run on (note: straylight is also in this list)
#SBATCH --time=16:00:00             # maximum time per job
#  USING JOB ARRAYS
#SBATCH --ntasks=1                  # number of tasks per job in array
#SBATCH --array=1-3                # number of jobs in array


module add slurm/14.03.0  openmpi/gcc/64/1.8.1 geant4/defaults root/5.34.32          # load whatever modules you need




srun -n 1 execute_geant.sh          # run command in parallel
