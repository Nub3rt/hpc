#!/bin/bash

#PBS -l select=1:ncpus=1:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q shortCPUQ

#PBS -o input.o
#PBS -e input.e
filename="input"

cd $PBS_O_WORKDIR

module load -s OpenMPI/4.1.1-GCC-11.2.0
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -Wall "${filename}.c" -o "${filename}.out"
fi
mpirun -n 1 "./${filename}.out" < input.in

# qsub input.sh | xargs -I {} watch "qstat -f {}"

