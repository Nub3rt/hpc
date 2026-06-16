#!/bin/bash

if [[ $# -lt 1 ]]; then
  echo "usage: $0 <img_path> [<placement_strategy>]"
  exit 1
fi

img_path=$1

for cores in 1 2 4 8 16; do
  for theta_multiplier in 1 2 4 8 16; do
    if [[ $# -gt 1 ]]; then
      ./mpi.sh "$cores" "$img_path" "$theta_multiplier" -p "$2" -t
    else
      ./mpi.sh "$cores" "$img_path" "$theta_multiplier" -t
    fi
  done
done
