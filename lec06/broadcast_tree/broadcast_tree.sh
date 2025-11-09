#!/bin/bash

#PBS -l select=4:ncpus=4:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

#PBS -o broadcast_tree.o
#PBS -e broadcast_tree.e
filename="broadcast_tree"

cd $PBS_O_WORKDIR

module load mpich-3.2
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -g -Wall -std=gnu99 "${filename}.c" -o "${filename}.out"
fi
mpirun.actual -n 16 "./${filename}.out"

# qsub broadcast_tree.sh | xargs -I {} watch "tracejob {}"

