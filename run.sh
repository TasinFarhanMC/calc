#!/bin/bash

set -e

DIR="$(dirname "$(realpath "$0")")"
cd "$DIR"

if [[ ! -d "$DIR/build" || "$1" ]]; then
  cmake --preset debug -G Ninja -DCALC_NUM_TYPE=Float64 -DCALC_TESTS=ON -DCALC_USE_STDLIB=OFF
fi

cmake --build --preset debug
cd build/debug
ctest -V
