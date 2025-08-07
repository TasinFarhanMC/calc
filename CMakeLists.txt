cmake_minimum_required(VERSION 3.11)
project(CALC C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

option(CALC_TESTS "Build Tests" OFF)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")

add_library(Calc INTERFACE ${SRC_DIR}/calc.h)
target_include_directories(Calc INTERFACE ${SRC_DIR})

add_library(CalcWithImpl STATIC ${SRC_DIR}/calc.c)
target_link_libraries(CalcWithImpl PRIVATE Calc)

function(add_macro name)
  if(DEFINED ${name})
    set(value "${${name}}")
    target_compile_definitions(Calc PUBLIC "${name}=${value}")
  endif()
endfunction()

add_macro(CALC_INT)
add_macro(CALC_FLOAT)

add_macro(CALC_WIDE_INT)
add_macro(CALC_USE_INT128)

add_macro(CALC_ALLOC)
add_macro(CALC_FREE)

add_macro(CALC_STACK_SIZE)
add_macro(CALC_MIN_ALLOC)

add_macro(CALC_IGNORE_UNKNOWN_CHAR)

if(CALC_TESTS)
  enable_language(CXX)

  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/get_cpm.cmake)
  cpmaddpackage("gh:catchorg/Catch2@3.9.0")

  file(GLOB_RECURSE TEST_FILES "${TEST_DIR}/*.cpp")
  add_executable(tests ${TEST_FILES})
  target_compile_features(tests PRIVATE cxx_std_17)
  target_link_libraries(tests PRIVATE Catch2::Catch2WithMain Calc)

  enable_testing()
  include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)
  catch_discover_tests(tests)
endif()
