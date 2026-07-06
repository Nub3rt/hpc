#!/bin/bash

mpi=false
omp=false

run=false

for arg in "$@"; do
  case "$arg" in
    --mpi) mpi=true ;;
    --omp) omp=true ;;
  esac
done

if $mpi; then
  python3 times.py mpi > times.mpi.txt
  python3 performance.py mpi > performance.mpi.txt
  run=true
  echo 'data processed for MPI'
fi

if $omp; then
  python3 times.py omp > times.omp.txt
  python3 performance.py omp > performance.omp.txt
  run=true
  echo 'data processed for OMP'
fi

if ! $run; then
  echo "usage: $0 [--mpi] [--omp]"
fi
