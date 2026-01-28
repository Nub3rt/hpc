#!/bin/bash

#PBS -l select=2:ncpus=4:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q shortCPUQ

#PBS -o open_ring.o
#PBS -e open_ring.e
filename="open_ring"

cd $PBS_O_WORKDIR

module load -s OpenMPI/4.1.1-GCC-11.2.0
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -Wall "${filename}.c" -o "${filename}.out"
fi
mpirun -n 8 "./${filename}.out"

# qsub open_ring.sh | xargs -I {} watch "qstat -f {}"

