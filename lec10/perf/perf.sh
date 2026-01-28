#!/bin/bash

#PBS -l select=2:ncpus=1:mem=2gb -l place=pack
#PBS -l walltime=0:01:00
#PBS -q shortCPUQ

#PBS -o perf.o
#PBS -e perf.e
filename="perf"

cd $PBS_O_WORKDIR

module load -s OpenMPI/4.1.1-GCC-11.2.0
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -Wall "${filename}.c" -o "${filename}.out"
fi
if [[ -f "${filename}.in" ]]; then
  mpirun -n 2 "./${filename}.out" < "${filename}.in"
else
  mpirun -n 2 "./${filename}.out"
fi

# qsub perf.sh | xargs -I {} watch "qstat -f {}"

