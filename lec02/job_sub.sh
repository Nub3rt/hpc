#!/bin/bash

#PBS -l select=1:ncpus=1:mem=2gb
#PBS -l walltime=0:00:10
#PBS -q short_cpuQ

sleep 5
ls -lrt
pwd
echo "name: " && $0

