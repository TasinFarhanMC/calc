#!/bin/bash

SRC_DIR="src"
BUILD_DIR="build"
SRC_FILE="$SRC_DIR/calc.s"
OBJ_FILE="$BUILD_DIR/calc.o"
OUT_FILE="$BUILD_DIR/calc"
RUN=$([[ "$1" == "r" || "$1" == "run" ]] && echo true)
shift

if [ ! -d "$BUILD_DIR" ]; then
  mkdir "$BUILD_DIR"
fi

if ! as "$SRC_FILE" -o "$OBJ_FILE"; then
  echo "Assembly failed."
  exit 1
fi

if ! ld -o "$OUT_FILE" -dynamic-linker "/lib64/ld-linux-x86-64.so.2" "$OBJ_FILE" "$@"; then
  echo "Linking failed."
  exit 1
fi

echo "Build successful: $OUT_FILE"

if [ "$RUN" ]; then
  echo "Running $OUT_FILE..."
  echo ""
  if ! "$OUT_FILE"; then
    echo "Execution failed."
    exit 1
  fi
fi
