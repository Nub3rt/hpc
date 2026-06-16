#!/bin/bash

if [[ $# -lt 1 ]]; then
  echo "usage: $0 <theta_multiplier> [<placement_strategy>]"
  exit 1
fi

theta_multiplier=$1

if [[ $# -gt 1 ]]; then
  placement="-p $2"
fi

for cores in 1 2 4 8 16; do
  for img_path in ../image/*; do
    ./mpi.sh "$cores" "$img_path" "$theta_multiplier" "$placement" -t
  done
done
