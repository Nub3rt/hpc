#!/bin/bash

#PBS -l select=4:ncpus=4:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q shortCPUQ

#PBS -o broadcast_tree.o
#PBS -e broadcast_tree.e
filename="broadcast_tree"

cd $PBS_O_WORKDIR

module load -s OpenMPI/4.1.1-GCC-11.2.0
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -Wall "${filename}.c" -o "${filename}.out"
fi
mpirun -n 16 "./${filename}.out"

# qsub broadcast_tree.sh | xargs -I {} watch "qstat -f {}"

