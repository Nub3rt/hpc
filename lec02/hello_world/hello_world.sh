#!/bin/bash

#PBS -l select=1:ncpus=1:mem=1gb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

#PBS -o hello_world.o
#PBS -e hello_world.e
filename="hello_world"

cd $PBS_O_WORKDIR

module load mpich-3.2
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -O2 "${filename}.c" -o "${filename}.out"
fi
mpirun.actual -n 1 "./${filename}.out"

