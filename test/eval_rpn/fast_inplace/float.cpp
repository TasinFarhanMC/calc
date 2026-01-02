#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
using namespace Catch;

#define CALC_STATIC_IMPLEMENTATION
#define CALC_FLOAT float
#include "calc.h"

#define FILL_NUMS(...)                                                                                                                                 \
  num_data.length = 0;                                                                                                                                 \
  {                                                                                                                                                    \
    CalcNum vals[] = {__VA_ARGS__};                                                                                                                    \
    for (size_t i = 0; i < sizeof(vals) / sizeof(vals[0]); i++) num_data.data[num_data.length++] = vals[i];                                            \
  }

#define FILL_CMDS(...)                                                                                                                                 \
  cmd_data.length = 0;                                                                                                                                 \
  {                                                                                                                                                    \
    CalcCmd cmds[] = {__VA_ARGS__};                                                                                                                    \
    for (size_t i = 0; i < sizeof(cmds) / sizeof(cmds[0]); i++) cmd_data.data[cmd_data.length++] = cmds[i];                                            \
  }

TEST_CASE("calc_evaluate_rpn_fast_inplace basic evaluation") {
  CalcNumData num_data;
  CalcNum num_buffer[16];
  num_data.data = num_buffer;
  num_data.capacity = 16;

  CalcCmdData cmd_data;
  CalcCmd cmd_buffer[16];
  cmd_data.data = cmd_buffer;
  cmd_data.capacity = 16;

  SECTION("1 + 2") {
    FILL_NUMS(1.0f, 2.0f);
    FILL_CMDS(CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_ADD);
    CalcNum res = calc_evaluate_rpn_fast_inplace(cmd_data, num_data);
    REQUIRE(res == Approx(3.0f));
  }

  SECTION("1 - 2") {
    FILL_NUMS(1.0f, 2.0f);
    FILL_CMDS(CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_SUB);
    CalcNum res = calc_evaluate_rpn_fast_inplace(cmd_data, num_data);
    REQUIRE(res == Approx(-1.0f));
  }

  SECTION("2 * 3") {
    FILL_NUMS(2.0f, 3.0f);
    FILL_CMDS(CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_MUL);
    CalcNum res = calc_evaluate_rpn_fast_inplace(cmd_data, num_data);
    REQUIRE(res == Approx(6.0f));
  }

  SECTION("6 / 3") {
    FILL_NUMS(6.0f, 3.0f);
    FILL_CMDS(CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_DIV);
    CalcNum res = calc_evaluate_rpn_fast_inplace(cmd_data, num_data);
    REQUIRE(res == Approx(2.0f));
  }

  SECTION("Unary negation") {
    FILL_NUMS(5.0f);
    FILL_CMDS(CALC_CMD_LOAD, CALC_CMD_NEG);
    CalcNum res = calc_evaluate_rpn_fast_inplace(cmd_data, num_data);
    REQUIRE(res == Approx(-5.0f));
  }

  SECTION("Complex expression 1 + 2 * 3") {
    FILL_NUMS(1.0f, 2.0f, 3.0f);
    FILL_CMDS(CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_MUL, CALC_CMD_ADD);
    CalcNum res = calc_evaluate_rpn_fast_inplace(cmd_data, num_data);
    REQUIRE(res == Approx(7.0f));
  }

  SECTION("Nested negation and subtraction: -(5 - 3)") {
    FILL_NUMS(5.0f, 3.0f);
    FILL_CMDS(CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_SUB, CALC_CMD_NEG);
    CalcNum res = calc_evaluate_rpn_fast_inplace(cmd_data, num_data);
    REQUIRE(res == Approx(-2.0f));
  }

  SECTION("Multiple operators: 1 + (2 - 3) * 4") {
    FILL_NUMS(1.0f, 2.0f, 3.0f, 4.0f);
    FILL_CMDS(CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_SUB, CALC_CMD_LOAD, CALC_CMD_MUL, CALC_CMD_ADD);
    CalcNum res = calc_evaluate_rpn_fast_inplace(cmd_data, num_data);
    REQUIRE(res == Approx(-3.0f));
  }
}
