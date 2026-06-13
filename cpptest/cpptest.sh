#!/bin/bash

#PBS -l select=1:ncpus=4:mem=2gb
#PBS -l walltime=0:01:00
#PBS -q shortCPUQ

#PBS -o cpptest.o
#PBS -e cpptest.e
filename="cpptest"

cd $PBS_O_WORKDIR
module load -s OpenMPI/4.1.1-GCC-11.2.0
if [[ "${filename}.cpp" -nt "${filename}.out" ]]; then
  mpic++ -Wall "${filename}.cpp" -o "${filename}.out"
fi
if [[ -f "${filename}.in" ]]; then
  mpirun -n 4 "./${filename}.out" < "${filename}.in"
else
  mpirun -n 4 "./${filename}.out"
fi

# qsub cpptest.sh | xargs -I {} watch "qstat -f {}"

