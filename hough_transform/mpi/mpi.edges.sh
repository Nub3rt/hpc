#!/bin/bash

if [[ $# -lt 1 ]]; then
  echo "usage: $0 <theta_multiplier> [<placement_strategy>]"
  exit 1
fi

theta_multiplier=$1

for cores in 1 2 4 8 16; do
  for img_path in ../image/*; do
    if [[ $# -gt 1 ]]; then
      ./mpi.sh "$cores" "$img_path" "$theta_multiplier" -p "$2" -t
    else
      ./mpi.sh "$cores" "$img_path" "$theta_multiplier" -t
    fi
  done
done
