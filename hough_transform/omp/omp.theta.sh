#!/bin/bash

if [[ $# -lt 1 ]]; then
  echo "usage: $0 <img_path> [<placement_strategy>]"
  exit 1
fi

img_path=$1

name=$(basename $0 .theta.sh)

if [[ "$name.c" -nt "$name.out" ]]; then
  module load GCC/11.2.0 GCCcore/11.2.0
  gcc -fopenmp -lm -O2 -Wall "$name.c" -o "$name.out" || { echo "Compilation failed, exiting..."; exit 1; }
fi

for nodes in 16 8 4 2 1; do
  for theta_multiplier in 16 8 4 2 1; do
    jobcount=$(qstat | grep norbert.*shortCPUQ | wc -l)
    while [[ $jobcount -gt 5 ]]; do
      sleep 20
      jobcount=$(qstat | grep norbert.*shortCPUQ | wc -l)
    done

    if [[ $# -gt 1 ]]; then
      "./$name.sh" "$nodes" "$img_path" "$theta_multiplier" -p "$2" -t
    else
      "./$name.sh" "$nodes" "$img_path" "$theta_multiplier" -t
    fi
  done
done
echo "submitted all jobs"
