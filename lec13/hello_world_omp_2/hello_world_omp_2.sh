#!/bin/bash

filename="hello_world_omp_2"
args="4"

if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  gcc -g -Wall -fopenmp "${filename}.c" -o "${filename}.out"
fi
if [[ -f "${filename}.in" ]]; then
  "./${filename}.out" $args  < "${filename}.in"
else
  "./${filename}.out" $args
fi

