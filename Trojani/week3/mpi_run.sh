#!/bin/bash

## Load the required modules
# module load mpt
# module load intel-compilers-19

mpicxx matrix_exercise.cpp -o matrix_EXE

# rm *.mod
# rm *.o

mpiexec -n $1 ./matrix_EXE $2
