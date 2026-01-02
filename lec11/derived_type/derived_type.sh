#!/bin/bash

#PBS -l select=1:ncpus=4:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

#PBS -o derived_type.o
#PBS -e derived_type.e
filename="derived_type"

cd $PBS_O_WORKDIR

module load mpich-3.2
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -g -Wall -std=gnu99 "${filename}.c" -o "${filename}.out"
fi
if [[ -f "${filename}.in" ]]; then
  mpirun.actual -n 4 "./${filename}.out" < "${filename}.in"
else
  mpirun.actual -n 4 "./${filename}.out"
fi

# qsub derived_type.sh | xargs -I {} watch "tracejob {}"

