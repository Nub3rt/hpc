#!/bin/bash

#PBS -l select=1:ncpus=4:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q shortCPUQ

#PBS -o base.o
#PBS -e base.e
filename="base"

cd $PBS_O_WORKDIR

module load -s OpenMPI/4.1.1-GCC-11.2.0
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -Wall "${filename}.c" -o "${filename}.out"
fi
if [[ -f "${filename}.in" ]]; then
  mpirun -n 4 "./${filename}.out" < "${filename}.in"
else
  mpirun -n 4 "./${filename}.out"
fi

# qsub base.sh | xargs -I {} watch "qstat -f {}"

