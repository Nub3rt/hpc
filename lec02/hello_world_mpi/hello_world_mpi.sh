#!/bin/bash

#PBS -l select=1:ncpus=4:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q shortCPUQ

#PBS -o hello_world_mpi.o
#PBS -e hello_world_mpi.e
filename="hello_world_mpi"

cd $PBS_O_WORKDIR

module load -s OpenMPI/4.1.1-GCC-11.2.0
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -O2 "${filename}.c" -o "${filename}.out"
fi
mpirun -n 4 "./${filename}.out"

