#!/bin/bash

#PBS -l select=1:ncpus=1:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

#PBS -o input.o
#PBS -e input.e
filename="input"

cd $PBS_O_WORKDIR

module load mpich-3.2
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -g -Wall -std=gnu99 "${filename}.c" -o "${filename}.out"
fi
mpirun.actual -n 1 "./${filename}.out" < input.in

# qsub input.sh | xargs -I {} watch "tracejob {}"

