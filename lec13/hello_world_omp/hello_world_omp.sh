#!/bin/bash

#PBS -l select=1:ncpus=4:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

#PBS -o hello_world_omp.o
#PBS -e hello_world_omp.e
filename="hello_world_omp"
args=""

cd $PBS_O_WORKDIR

if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  gcc -g -Wall -fopenmp "${filename}.c" -o "${filename}.out"
fi

if [[ -f "${filename}.in" ]]; then
  "./${filename}.out" $args  < "${filename}.in"
else
  "./${filename}.out" $args
fi

# qsub hello_world_omp.sh | xargs -I {} watch "tracejob {}"

