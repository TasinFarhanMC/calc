cmake_minimum_required(VERSION 3.11)
project(CALC C)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIRS "${SRC_DIR}")

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.c")

add_executable(calc "${SRC_FILES}")
target_include_directories(calc PRIVATE ${INCLUDE_DIRS})
