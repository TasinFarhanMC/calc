cmake_minimum_required(VERSION 3.11)
project(CALC C CXX) # Enable both C and C++

set(CALC_NUM_TYPE
    "X32"
    CACHE STRING "Fixed Point Width")
set_property(CACHE CALC_NUM_TYPE PROPERTY STRINGS "X32" "X64" "F32" "F64" "F80")

option(CALC_TESTS "Build Tests" OFF)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCL_DIR "${SRC_DIR}")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")

# Include directories
include_directories(${SRC_DIR})

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.c" "${SRC_DIR}/*.h")
add_library(calc ${SRC_FILES})
target_include_directories(calc PUBLIC ${INCL_DIR})

if(${CALC_NUM_TYPE} EQUAL "F32" OR "F64")
  target_compile_definitions(calc PUBLIC CALC_NUM_FLOAT
                                         CALC_NUM_TYPE=${CALC_NUM_TYPE})
else()
  target_compile_definitions(calc PUBLIC CALC_NUM_FIXED
                                         CALC_NUM_TYPE=${CALC_NUM_TYPE})
endif()

if(CALC_TESTS) # ✅ Fixed the typo
  include(CTest)
  enable_testing()
  file(GLOB_RECURSE TEST_FILES "${TEST_DIR}/*.cpp" "${TEST_DIR}/*.c")

  foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE} ${SRC_FILES})
    target_link_libraries(${TEST_NAME} calc)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
  endforeach()
endif()
