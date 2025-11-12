#!/bin/bash

#PBS -l select=1:ncpus=4:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

#PBS -o trap_1.o
#PBS -e trap_1.e
filename="trap_1"
args="4"

cd $PBS_O_WORKDIR

if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  gcc -g -Wall -std=gnu99 -fopenmp "${filename}.c" -o "${filename}.out"
fi

if [[ -f "${filename}.in" ]]; then
  "./${filename}.out" $args  < "${filename}.in"
else
  "./${filename}.out" $args
fi

# qsub trap_1.sh | xargs -I {} watch "tracejob {}"

