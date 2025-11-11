#!/bin/bash

filename="hello_world_omp"

if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  gcc -g -Wall -fopenmp "${filename}.c" -o "${filename}.out"
fi
if [[ -f "${filename}.in" ]]; then
  "./${filename}.out" < "${filename}.in"
else
  "./${filename}.out"
fi

