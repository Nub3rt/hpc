#!/bin/bash

#PBS -l select=1:ncpus=2:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q shortCPUQ

#PBS -o ping_pong.o
#PBS -e ping_pong.e
filename="ping_pong"

cd $PBS_O_WORKDIR

module load -s OpenMPI/4.1.1-GCC-11.2.0
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -Wall "${filename}.c" -o "${filename}.out"
fi
mpirun -n 2 "./${filename}.out"

# qsub ping_pong.sh | xargs -I {} watch "qstat -f {}"

