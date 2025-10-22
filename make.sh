#!/bin/bash

make all && make clean

valgrind --leak-check=full --show-leak-kinds=all -s ./bin/run "$@"
# ./bin/run "$@"

# stdbuf -oL -eL bash make.sh | less
