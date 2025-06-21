#include "calc.h"
#include "test.hpp"

START_TEST();

auto make_cmds = [](CalcCmd *arr, CalcU16 size) -> CalcCmds { return CalcCmds {arr, size}; };
auto make_nums = [](CalcNum *arr, CalcU16 size) -> CalcNums { return CalcNums {arr, size}; };

auto approx_equal = [](float a, float b, float epsilon = 0.0001f) -> bool { return (a > b ? a - b : b - a) < epsilon; };

// Test 1: Simple addition: 2 3 + => 5
{
  CalcCmd cmds[] = {CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_ADD};
  CalcNum nums[] = {{2.0f}, {3.0f}};

  auto name = "2 3 +";

  CalcBufs bufs;
  bufs.cmds = make_cmds(cmds, 3);
  bufs.nums = make_nums(nums, 2);

  auto result = calc_eval_rpn(bufs);

  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Result value", approx_equal(result.ok.val, 5.0f));
}

// Test 2: Simple subtraction: 5 2 - => 3
{
  CalcCmd cmds[] = {CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_SUB};
  CalcNum nums[] = {{5.0f}, {2.0f}};

  auto name = "5 2 -";

  CalcBufs bufs;
  bufs.cmds = make_cmds(cmds, 3);
  bufs.nums = make_nums(nums, 2);

  auto result = calc_eval_rpn(bufs);

  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Result value", approx_equal(result.ok.val, 3.0f));
}

// Test 3: Multiplication: 3 4 * => 12
{
  CalcCmd cmds[] = {CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_MUL};
  CalcNum nums[] = {{3.0f}, {4.0f}};

  auto name = "3 4 *";

  CalcBufs bufs;
  bufs.cmds = make_cmds(cmds, 3);
  bufs.nums = make_nums(nums, 2);

  auto result = calc_eval_rpn(bufs);

  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Result value", approx_equal(result.ok.val, 12.0f));
}

// Test 4: Division: 10 2 / => 5
{
  CalcCmd cmds[] = {CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_DIV};
  CalcNum nums[] = {{10.0f}, {2.0f}};

  auto name = "10 2 /";

  CalcBufs bufs;
  bufs.cmds = make_cmds(cmds, 3);
  bufs.nums = make_nums(nums, 2);

  auto result = calc_eval_rpn(bufs);

  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Result value", approx_equal(result.ok.val, 5.0f));
}

// Test 5: Negation: 3 NEG => -3
{
  CalcCmd cmds[] = {CALC_CMD_LOAD, CALC_CMD_NEG};
  CalcNum nums[] = {{3.0f}};

  auto name = "3 NEG";

  CalcBufs bufs;
  bufs.cmds = make_cmds(cmds, 2);
  bufs.nums = make_nums(nums, 1);

  auto result = calc_eval_rpn(bufs);

  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Result value", approx_equal(result.ok.val, -3.0f));
}

// Test 6: Complex expression: (2 + 3) * 4 = 20
// RPN: 2 3 + 4 *
{
  CalcCmd cmds[] = {CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_ADD, CALC_CMD_LOAD, CALC_CMD_MUL};
  CalcNum nums[] = {{2.0f}, {3.0f}, {4.0f}};

  auto name = "(2 + 3) * 4";

  CalcBufs bufs;
  bufs.cmds = make_cmds(cmds, 5);
  bufs.nums = make_nums(nums, 3);

  auto result = calc_eval_rpn(bufs);

  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Result value", approx_equal(result.ok.val, 20.0f));
}

// Test 7: Division by zero error (depends on your calc_div_num implementation)
// This test assumes that division by zero produces an error != CALC_ERR_NONE
{
  CalcCmd cmds[] = {CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_DIV};
  CalcNum nums[] = {{5.0f}, {0.0f}};

  auto name = "5 0 / (div by zero)";

  CalcBufs bufs;
  bufs.cmds = make_cmds(cmds, 3);
  bufs.nums = make_nums(nums, 2);

  auto result = calc_eval_rpn(bufs);

  TEST_CASE("Error returned", result.err != CALC_ERR_NONE);
}

// Test 8: Stack underflow: not enough operands for +
{
  CalcCmd cmds[] = {CALC_CMD_LOAD, CALC_CMD_ADD};
  CalcNum nums[] = {{5.0f}};

  auto name = "5 + (missing operand)";

  CalcBufs bufs;
  bufs.cmds = make_cmds(cmds, 2);
  bufs.nums = make_nums(nums, 1);

  auto result = calc_eval_rpn(bufs);

  TEST_CASE("Error returned", result.err == CALC_ERR_INV_SYNTAX);
}

// Test 9: Empty input (no commands)
{
  CalcCmd *cmds = nullptr;
  CalcNum *nums = nullptr;

  auto name = "empty input";

  CalcBufs bufs;
  bufs.cmds = make_cmds(cmds, 0);
  bufs.nums = make_nums(nums, 0);

  auto result = calc_eval_rpn(bufs);

  TEST_CASE("Error returned", result.err == CALC_ERR_INV_SYNTAX);
}

// Test 10: Negation of sum: 2 3 + NEG = -5
{
  CalcCmd cmds[] = {CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_ADD, CALC_CMD_NEG};
  CalcNum nums[] = {{2.0f}, {3.0f}};

  auto name = "negation of 2 + 3";

  CalcBufs bufs;
  bufs.cmds = make_cmds(cmds, 4);
  bufs.nums = make_nums(nums, 2);

  auto result = calc_eval_rpn(bufs);

  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Result value", approx_equal(result.ok.val, -5.0f));
}

// Test 11: Excess numbers (should ignore extra nums, only use needed)
{
  CalcCmd cmds[] = {CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_ADD};
  CalcNum nums[] = {{1.0f}, {2.0f}, {3.0f}, {4.0f}}; // Extra numbers beyond commands

  auto name = "excess numbers";

  CalcBufs bufs;
  bufs.cmds = make_cmds(cmds, 3);
  bufs.nums = make_nums(nums, 4);

  auto result = calc_eval_rpn(bufs);

  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Result value", approx_equal(result.ok.val, 3.0f));
}

// Test 12: Insufficient numbers for LOAD commands
{
  CalcCmd cmds[] = {CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_ADD};
  CalcNum nums[] = {{1.0f}}; // only one number, but two LOAD commands

  auto name = "insufficient numbers";

  CalcBufs bufs;
  bufs.cmds = make_cmds(cmds, 3);
  bufs.nums = make_nums(nums, 1);

  auto result = calc_eval_rpn(bufs);

  TEST_CASE("Error returned", result.err == CALC_ERR_INV_SYNTAX);
}

// Test 13: Only one number loaded, no operators
{
  CalcCmd cmds[] = {CALC_CMD_LOAD};
  CalcNum nums[] = {{42.0f}};

  auto name = "single number";

  CalcBufs bufs;
  bufs.cmds = make_cmds(cmds, 1);
  bufs.nums = make_nums(nums, 1);

  auto result = calc_eval_rpn(bufs);

  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Result value", approx_equal(result.ok.val, 42.0f));
}

// Test 14: Invalid operator in cmds (e.g. CALC_CMD_L_BRACE)
{
  CalcCmd cmds[] = {CALC_CMD_LOAD, CALC_CMD_L_BRACE, CALC_CMD_LOAD};
  CalcNum nums[] = {{1.0f}, {2.0f}};

  auto name = "invalid operator";

  CalcBufs bufs;
  bufs.cmds = make_cmds(cmds, 3);
  bufs.nums = make_nums(nums, 2);

  auto result = calc_eval_rpn(bufs);

  TEST_CASE("Error returned", result.err == CALC_ERR_INV_SYNTAX);
}

END_TEST();
