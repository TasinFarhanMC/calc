cmake_minimum_required(VERSION 3.11)
project(CALC C CXX) # Enable both C and C++

set(CALC_NUM_TYPE
    "Fixed32"
    CACHE STRING "Numeric Type (Fixed32, Fixed64, Float32, Float64, Float80)")
set_property(CACHE CALC_NUM_TYPE PROPERTY STRINGS "Fixed32" "Fixed64" "Float32"
                                          "Float64" "Float80")

set(CALC_BUF_SIZE
    "1024"
    CACHE STRING "Max Buffer item size [128, 65535]")
# Validate buffer size
if(CALC_BUF_SIZE LESS 128 OR CALC_BUF_SIZE GREATER 65535)
  message(
    FATAL_ERROR
      "CALC_BUF_SIZE must be between 128 and 65535. Current value: ${CALC_BUF_SIZE}"
  )
endif()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

option(CALC_TESTS "Build Tests" OFF)
option(CALC_USE_STDLIB "Use C stdlib instead of custom definitions" OFF)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCL_DIR "${SRC_DIR}")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.c" "${SRC_DIR}/*.h")
add_library(calc ${SRC_FILES})
target_include_directories(calc PUBLIC ${INCL_DIR})

if(CALC_NUM_TYPE STREQUAL "Fixed32")
  target_compile_definitions(calc PUBLIC CALC_NUM_FIXED CALC_NUM_WIDTH=32)
elseif(CALC_NUM_TYPE STREQUAL "Fixed64")
  target_compile_definitions(calc PUBLIC CALC_NUM_FIXED CALC_NUM_WIDTH=64)
elseif(CALC_NUM_TYPE STREQUAL "Float32")
  target_compile_definitions(calc PUBLIC CALC_NUM_FLOAT CALC_NUM_WIDTH=32)
elseif(CALC_NUM_TYPE STREQUAL "Float64")
  target_compile_definitions(calc PUBLIC CALC_NUM_FLOAT CALC_NUM_WIDTH=64)
elseif(CALC_NUM_TYPE STREQUAL "Float80")
  target_compile_definitions(calc PUBLIC CALC_NUM_FLOAT CALC_NUM_WIDTH=80)
endif()

if(CALC_USE_STDLIB)
  target_compile_definitions(calc PUBLIC CALC_USE_STDLIB)
endif()

target_compile_definitions(calc PUBLIC CALC_BUF_SIZE=${CALC_BUF_SIZE})

if(CALC_TESTS)
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/get_cpm.cmake)

  cpmaddpackage("gh:catchorg/Catch2@3.9.0")

  file(GLOB_RECURSE TEST_FILES "${TEST_DIR}/*.cpp")
  add_executable(tests ${TEST_FILES})
  target_compile_features(tests PRIVATE cxx_std_17)
  target_link_libraries(tests PRIVATE Catch2::Catch2WithMain calc)

  enable_testing()
  include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)
  catch_discover_tests(tests)
endif()
