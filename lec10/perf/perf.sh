#!/bin/bash

#PBS -l select=2:ncpus=1:mem=2gb -l place=pack
#PBS -l walltime=0:01:00
#PBS -q short_cpuQ

#PBS -o perf.o
#PBS -e perf.e
filename="perf"

cd $PBS_O_WORKDIR

module load mpich-3.2
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -g -Wall -std=gnu99 "${filename}.c" -o "${filename}.out"
fi
if [[ -f "${filename}.in" ]]; then
  mpirun.actual -n 2 "./${filename}.out" < "${filename}.in"
else
  mpirun.actual -n 2 "./${filename}.out"
fi

# qsub perf.sh | xargs -I {} watch "tracejob {}"

