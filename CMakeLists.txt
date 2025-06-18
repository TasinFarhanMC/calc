cmake_minimum_required(VERSION 3.11)
project(CALC C CXX) # Enable both C and C++

option(WITH_TESTS "Build tests" OFF)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")

# Include directories
include_directories(${SRC_DIR})

# Gather main source files (C)
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.c" "${SRC_DIR}/*.h")

# Main application
add_library(calc ${SRC_FILES})
target_compile_options(calc PRIVATE -ffreestanding -nostdlib -nostdinc)
target_link_options(calc PRIVATE -nostdlib)

if(WITH_TESTS)
  enable_testing()
  file(GLOB_RECURSE TEST_FILES "${TEST_DIR}/*.cpp")

  foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE} ${SRC_FILES})
    target_link_libraries(${TEST_NAME} calc)
    target_include_directories(${TEST_NAME} PRIVATE ${SRC_DIR})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
  endforeach()
endif()
