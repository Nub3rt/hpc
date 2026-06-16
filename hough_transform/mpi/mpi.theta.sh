#!/bin/bash

if [[ $# -lt 1 ]]; then
  echo "usage: $0 <img_path> [<placement_strategy>]"
  exit 1
fi

img_path=$1

if [[ $# -gt 1 ]]; then
  placement="-p $2"
fi

for cores in 1 2 4 8 16; do
  for theta_multiplier in 1 2 4 8 16; do
    ./mpi.sh "$cores" "$img_path" "$theta_multiplier" "$placement" -t
  done
done
