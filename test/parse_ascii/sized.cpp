#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
using namespace Catch;

#define CALC_STATIC_IMPLEMENTATION
#define CALC_FLOAT float
#include "calc.h"

TEST_CASE("calc_parse_ascii basic numbers") {
  CalcNum nums_buffer[16];
  CalcCmd cmds_buffer[16];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 16);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 16);

  const char *expr = "123";
  CalcError err = calc_parse_ascii((const CalcByte *)expr, strlen(expr), &nums, &cmds);

  REQUIRE(err == CALC_ERROR_NONE);
  REQUIRE(nums.length == 1);
  REQUIRE(nums.data[0] == 123);
  REQUIRE(cmds.length == 1);
  REQUIRE(cmds.data[0] == CALC_CMD_LOAD);
}

TEST_CASE("calc_parse_ascii simple operations") {
  CalcNum nums_buffer[16];
  CalcCmd cmds_buffer[16];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 16);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 16);

  const char *expr = "3+4*2";
  CalcError err = calc_parse_ascii((const CalcByte *)expr, strlen(expr), &nums, &cmds);

  REQUIRE(err == CALC_ERROR_NONE);
  REQUIRE(nums.length == 3);
  REQUIRE(nums.data[0] == 3);
  REQUIRE(nums.data[1] == 4);
  REQUIRE(nums.data[2] == 2);
  REQUIRE(cmds.length == 5);
  REQUIRE(cmds.data[0] == CALC_CMD_LOAD);
  REQUIRE(cmds.data[1] == CALC_CMD_ADD);
  REQUIRE(cmds.data[2] == CALC_CMD_LOAD);
  REQUIRE(cmds.data[3] == CALC_CMD_MUL);
  REQUIRE(cmds.data[4] == CALC_CMD_LOAD);
}

TEST_CASE("calc_parse_ascii parentheses") {
  CalcNum nums_buffer[16];
  CalcCmd cmds_buffer[16];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 16);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 16);

  const char *expr = "(1+2)*3";
  CalcError err = calc_parse_ascii((const CalcByte *)expr, strlen(expr), &nums, &cmds);

  REQUIRE(err == CALC_ERROR_NONE);
  REQUIRE(nums.length == 3);
  REQUIRE(nums.data[0] == 1);
  REQUIRE(nums.data[1] == 2);
  REQUIRE(nums.data[2] == 3);
  REQUIRE(cmds.length == 7);
  REQUIRE(cmds.data[0] == CALC_CMD_L_BRACE);
  REQUIRE(cmds.data[1] == CALC_CMD_LOAD);
  REQUIRE(cmds.data[2] == CALC_CMD_ADD);
  REQUIRE(cmds.data[3] == CALC_CMD_LOAD);
  REQUIRE(cmds.data[4] == CALC_CMD_R_BRACE);
  REQUIRE(cmds.data[5] == CALC_CMD_MUL);
  REQUIRE(cmds.data[6] == CALC_CMD_LOAD);
}

TEST_CASE("calc_parse_ascii negative numbers") {
  CalcNum nums_buffer[16];
  CalcCmd cmds_buffer[16];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 16);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 16);

  const char *expr = "-5+6";
  CalcError err = calc_parse_ascii((const CalcByte *)expr, strlen(expr), &nums, &cmds);

  REQUIRE(err == CALC_ERROR_NONE);
  REQUIRE(nums.length == 2);
  REQUIRE(nums.data[0] == -5);
  REQUIRE(nums.data[1] == 6);
  REQUIRE(cmds.length == 3);
  REQUIRE(cmds.data[0] == CALC_CMD_LOAD);
  REQUIRE(cmds.data[1] == CALC_CMD_ADD);
  REQUIRE(cmds.data[2] == CALC_CMD_LOAD);
}

TEST_CASE("calc_parse_ascii unknown symbols") {
  CalcNum nums_buffer[16];
  CalcCmd cmds_buffer[16];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 16);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 16);

  const char *expr = "3$4";
  CalcError err = calc_parse_ascii((const CalcByte *)expr, strlen(expr), &nums, &cmds);

  REQUIRE(err == CALC_ERROR_UNKNOWN_SYMBOL);
}

TEST_CASE("calc_parse_ascii consecutive unary operators") {
  CalcNum nums_buffer[16];
  CalcCmd cmds_buffer[16];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 16);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 16);

  const char *expr = "--5 + +3";
  CalcError err = calc_parse_ascii((const CalcByte *)expr, strlen(expr), &nums, &cmds);

  REQUIRE(err == CALC_ERROR_NONE);
  REQUIRE(nums.length == 2);
  REQUIRE(nums.data[0] == 5); // --5 -> 5
  REQUIRE(nums.data[1] == 3); // +3 -> 3
  REQUIRE(cmds.length == 3);
  REQUIRE(cmds.data[0] == CALC_CMD_LOAD);
  REQUIRE(cmds.data[1] == CALC_CMD_ADD);
  REQUIRE(cmds.data[2] == CALC_CMD_LOAD);
}

TEST_CASE("calc_parse_ascii nested parentheses") {
  CalcNum nums_buffer[16];
  CalcCmd cmds_buffer[16];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 16);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 16);

  const char *expr = "((1+2)*(3-4))/5";
  CalcError err = calc_parse_ascii((const CalcByte *)expr, strlen(expr), &nums, &cmds);

  REQUIRE(err == CALC_ERROR_NONE);
  REQUIRE(nums.length == 5);
  REQUIRE(nums.data[0] == 1);
  REQUIRE(nums.data[1] == 2);
  REQUIRE(nums.data[2] == 3);
  REQUIRE(nums.data[3] == 4);
  REQUIRE(nums.data[4] == 5);
  REQUIRE(cmds.length == 15);
  REQUIRE(cmds.data[0] == CALC_CMD_L_BRACE);
  REQUIRE(cmds.data[1] == CALC_CMD_L_BRACE);
  REQUIRE(cmds.data[2] == CALC_CMD_LOAD); // 1
  REQUIRE(cmds.data[3] == CALC_CMD_ADD);
  REQUIRE(cmds.data[4] == CALC_CMD_LOAD); // 2
  REQUIRE(cmds.data[5] == CALC_CMD_R_BRACE);
  REQUIRE(cmds.data[6] == CALC_CMD_MUL);
  REQUIRE(cmds.data[7] == CALC_CMD_L_BRACE);
  REQUIRE(cmds.data[8] == CALC_CMD_LOAD); // 3
  REQUIRE(cmds.data[9] == CALC_CMD_SUB);
  REQUIRE(cmds.data[10] == CALC_CMD_LOAD); // 4
  REQUIRE(cmds.data[11] == CALC_CMD_R_BRACE);
  REQUIRE(cmds.data[12] == CALC_CMD_R_BRACE);
  REQUIRE(cmds.data[13] == CALC_CMD_DIV);
  REQUIRE(cmds.data[14] == CALC_CMD_LOAD); // 5
}

TEST_CASE("calc_parse_ascii buffer limits") {
  CalcNum nums_buffer[4];
  CalcCmd cmds_buffer[4];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 4);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 4);

  const char *expr = "1 + 2 + 3 +4+5";
  CalcError err = calc_parse_ascii((const CalcByte *)expr, strlen(expr), &nums, &cmds);

  REQUIRE(err == CALC_ERROR_DATA_OVERFLOW); // buffers too small
}

TEST_CASE("calc_parse_ascii complex mixed operators") {
  CalcNum nums_buffer[16];
  CalcCmd cmds_buffer[16];
  CalcNumData nums = CALC_NUM_DATA(nums_buffer, 16);
  CalcCmdData cmds = CALC_CMD_DATA(cmds_buffer, 16);

  const char *expr = "3+4*2/(1-5)^2";
  CalcError err = calc_parse_ascii((const CalcByte *)expr, strlen(expr), &nums, &cmds);

  REQUIRE(err == CALC_ERROR_UNKNOWN_SYMBOL); // ^ not supported
}
