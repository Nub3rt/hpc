#!/bin/bash

#PBS -l select=2:ncpus=4:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

#PBS -o open_ring.o
#PBS -e open_ring.e
filename="open_ring"

cd $PBS_O_WORKDIR

module load mpich-3.2
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -g -Wall -std=gnu99 "${filename}.c" -o "${filename}.out"
fi
mpirun.actual -n 8 "./${filename}.out"

# qsub open_ring.sh | xargs -I {} watch "tracejob {}"

