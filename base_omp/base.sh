#!/bin/bash

filename="base"
args=""

if [[ "${filename}.c" -nt "${filename}.out" ]]; then
  gcc -g -Wall -fopenmp "${filename}.c" -o "${filename}.out"
fi
if [[ -f "${filename}.in" ]]; then
  "./${filename}.out" $args < "${filename}.in"
else
  "./${filename}.out" $args
fi

