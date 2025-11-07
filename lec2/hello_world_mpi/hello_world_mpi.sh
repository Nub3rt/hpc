#!/bin/bash

#PBS -l select=1:ncpus=4:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

#PBS -o hello_world_mpi.o
#PBS -e hello_world_mpi.e
filename="hello_world_mpi"

cd $PBS_O_WORKDIR

module load mpich-3.2
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -O2 "${filename}.c" -o "${filename}.out"
fi
mpirun.actual -n 4 "./${filename}.out"

