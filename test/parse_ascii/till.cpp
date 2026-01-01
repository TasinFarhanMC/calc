#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
using namespace Catch;

#define CALC_STATIC_IMPLEMENTATION
#define CALC_FLOAT float
#include "calc.h"

TEST_CASE("calc_parse_ascii_till stops at first delimiter") {
  CalcNum nums_buffer[16];
  CalcCmd cmds_buffer[16];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 16);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 16);

  const char *expr = "1+2;3+4";
  CalcError err = calc_parse_ascii_till(
      (const CalcByte *)expr,
      (const CalcByte *)(expr + strlen(expr)), // stop pointer
      ';', 1, &nums, &cmds
  );

  REQUIRE(err == CALC_ERROR_NONE);
  REQUIRE(nums.length == 2);
  REQUIRE(nums.data[0] == 1);
  REQUIRE(nums.data[1] == 2);
  REQUIRE(cmds.length == 3);
  REQUIRE(cmds.data[0] == CALC_CMD_LOAD);
  REQUIRE(cmds.data[1] == CALC_CMD_ADD);
  REQUIRE(cmds.data[2] == CALC_CMD_LOAD);
}

TEST_CASE("calc_parse_ascii_till stops at Nth delimiter") {
  CalcNum nums_buffer[16];
  CalcCmd cmds_buffer[16];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 16);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 16);

  const char *expr = "1+2;3+4;5+6";
  CalcError err = calc_parse_ascii_till((const CalcByte *)expr, (const CalcByte *)(expr + strlen(expr)), ';', 2, &nums, &cmds);

  REQUIRE(err == CALC_ERROR_NONE);
  REQUIRE(nums.length == 4);
  REQUIRE(nums.data[0] == 1);
  REQUIRE(nums.data[1] == 2);
  REQUIRE(nums.data[2] == 3);
  REQUIRE(nums.data[3] == 4);
  REQUIRE(cmds.length == 6);
}

TEST_CASE("calc_parse_ascii_till zero count parses nothing") {
  CalcNum nums_buffer[8];
  CalcCmd cmds_buffer[8];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 8);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 8);

  const char *expr = "1+2+3";
  CalcError err = calc_parse_ascii_till((const CalcByte *)expr, (const CalcByte *)(expr + strlen(expr)), '+', 0, &nums, &cmds);

  REQUIRE(err == CALC_ERROR_NONE);
  REQUIRE(nums.length == 0);
  REQUIRE(cmds.length == 0);
}

TEST_CASE("calc_parse_ascii_till delimiter not found") {
  CalcNum nums_buffer[16];
  CalcCmd cmds_buffer[16];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 16);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 16);

  const char *expr = "1+2+3";
  CalcError err = calc_parse_ascii_till(
      (const CalcByte *)expr,
      (const CalcByte *)(expr + strlen(expr)), // scan full string safely
      ';', 1, &nums, &cmds
  );

  REQUIRE(err == CALC_ERROR_NONE);
  REQUIRE(nums.length == 3);
  REQUIRE(nums.data[0] == 1);
  REQUIRE(nums.data[1] == 2);
  REQUIRE(nums.data[2] == 3);
}

TEST_CASE("calc_parse_ascii_till handles nested expressions before delimiter") {
  CalcNum nums_buffer[16];
  CalcCmd cmds_buffer[16];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 16);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 16);

  const char *expr = "(1+2)*(3-4);5";
  CalcError err = calc_parse_ascii_till((const CalcByte *)expr, (const CalcByte *)(expr + strlen(expr)), ';', 1, &nums, &cmds);

  REQUIRE(err == CALC_ERROR_NONE);
  REQUIRE(nums.length == 4);
  REQUIRE(nums.data[0] == 1);
  REQUIRE(nums.data[1] == 2);
  REQUIRE(nums.data[2] == 3);
  REQUIRE(nums.data[3] == 4);
}

TEST_CASE("calc_parse_ascii_till unknown symbol before delimiter") {
  CalcNum nums_buffer[16];
  CalcCmd cmds_buffer[16];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 16);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 16);

  const char *expr = "1+2$3;4";
  CalcError err = calc_parse_ascii_till((const CalcByte *)expr, (const CalcByte *)(expr + strlen(expr)), ';', 1, &nums, &cmds);

  REQUIRE(err == CALC_ERROR_UNKNOWN_SYMBOL);
}

TEST_CASE("calc_parse_ascii_till buffer overflow before delimiter") {
  CalcNum nums_buffer[2];
  CalcCmd cmds_buffer[2];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 2);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 2);

  const char *expr = "1+2+3;4";
  CalcError err = calc_parse_ascii_till((const CalcByte *)expr, (const CalcByte *)(expr + strlen(expr)), ';', 1, &nums, &cmds);

  REQUIRE(err == CALC_ERROR_DATA_OVERFLOW);
}

TEST_CASE("calc_parse_ascii_till unary operators before delimiter") {
  CalcNum nums_buffer[8];
  CalcCmd cmds_buffer[8];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 8);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 8);

  const char *expr = "--5+-3;7";
  CalcError err = calc_parse_ascii_till((const CalcByte *)expr, (const CalcByte *)(expr + strlen(expr)), ';', 1, &nums, &cmds);

  REQUIRE(err == CALC_ERROR_NONE);
  REQUIRE(nums.length == 2);
  REQUIRE(nums.data[0] == 5);
  REQUIRE(nums.data[1] == -3);
}

TEST_CASE("calc_parse_ascii_till delimiter missing") {
  CalcNum nums_buffer[16];
  CalcCmd cmds_buffer[16];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 16);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 16);

  const char *expr = "1+2+3"; // no ';' present
  CalcError err = calc_parse_ascii_till(
      (const CalcByte *)expr,
      (const CalcByte *)(expr + strlen(expr)), // parse to end safely
      ';',
      1, // looking for 1 occurrence of ';'
      &nums, &cmds
  );

  REQUIRE(err == CALC_ERROR_NONE); // should succeed
  REQUIRE(nums.length == 3);       // all numbers parsed
  REQUIRE(nums.data[0] == 1);
  REQUIRE(nums.data[1] == 2);
  REQUIRE(nums.data[2] == 3);

  REQUIRE(cmds.length == 5); // all commands before missing delimiter
  REQUIRE(cmds.data[0] == CALC_CMD_LOAD);
  REQUIRE(cmds.data[1] == CALC_CMD_ADD);
  REQUIRE(cmds.data[2] == CALC_CMD_LOAD);
  REQUIRE(cmds.data[3] == CALC_CMD_ADD);
  REQUIRE(cmds.data[4] == CALC_CMD_LOAD);
}
