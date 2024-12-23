#!/usr/bin/env bash

set -eoux pipefail

day=$1

docker run --rm -v "$PWD":/usr/src/aoc -w /usr/src/aoc gcc:latest g++ -std=c++17 ${day}.cpp -o ${day}

#docker run --rm -v "$PWD":/usr/src/aoc -w /usr/src/aoc gcc:latest ./${day}
