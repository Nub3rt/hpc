#!/bin/bash

#PBS -l select=1:ncpus=2:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

#PBS -o ping_pong.o
#PBS -e ping_pong.e
filename="ping_pong"

cd $PBS_O_WORKDIR

module load mpich-3.2
if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  mpicc -g -Wall -std=gnu99 "${filename}.c" -o "${filename}.out"
fi
mpirun.actual -n 2 "./${filename}.out"

# qsub ping_pong.sh | xargs -I {} watch "tracejob {}"

