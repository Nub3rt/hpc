#!/bin/bash

#PBS -l select=1:ncpus=4:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

#PBS -o vec_add.o
#PBS -e vec_add.e
filename="vec_add"

cd $PBS_O_WORKDIR

module load mpich-3.2
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -g -Wall -std=gnu99 "${filename}.c" -o "${filename}.out"
fi
mpirun.actual -n 4 "./${filename}.out" < "${filename}.in"

# qsub vec_add.sh | xargs -I {} watch "tracejob {}"

