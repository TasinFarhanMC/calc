#include "test.hpp"
#include <iostream>

#include "calc.h"

START_TEST();

auto make_cmds = [](CalcCmd *arr, CalcU16 size) -> CalcCmds { return CalcCmds {arr, size}; };

auto cmds_equal = [](const CalcCmds a, const CalcCmds b) -> bool {
  if (a.size != b.size) return false;
  for (CalcU16 i = 0; i < a.size; i++) {
    if (a.data[i] != b.data[i]) return false;
  }
  return true;
};

auto print_cmds = [](const char *label, const CalcCmds cmds) {
  std::cerr << label << ": ";
  for (CalcU16 i = 0; i < cmds.size; i++) { std::cerr << cmds.data[i] << " "; }
  std::cerr << "\n";
};

// Test 1: Basic operator precedence (5 + 7)
{
  CalcCmd input[] = {CALC_CMD_LOAD, CALC_CMD_ADD, CALC_CMD_LOAD};
  CalcCmd expected[] = {CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_ADD};

  NAME("5 + 7");
  auto result = calc_gen_rpn(make_cmds(input, 3));
  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Output size", result.ok.size == 3);
  TEST_CASE("Match expected", cmds_equal(result.ok, make_cmds(expected, 3)));
}

// Test 2: Empty input
{
  CalcCmd input[] = {};
  NAME("Empty input");
  auto result = calc_gen_rpn(make_cmds(input, 0));
  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Empty output", result.ok.size == 0);
}

// Test 3: (2 + 3) * 4
{
  CalcCmd input[] = {CALC_CMD_L_BRACE, CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_ADD, CALC_CMD_R_BRACE, CALC_CMD_LOAD, CALC_CMD_MUL};
  CalcCmd expected[] = {CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_ADD, CALC_CMD_LOAD, CALC_CMD_MUL};

  NAME("(2 + 3) * 4");
  auto result = calc_gen_rpn(make_cmds(input, 7));
  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Size", result.ok.size == 5);
  TEST_CASE("Match", cmds_equal(result.ok, make_cmds(expected, 5)));
}

// Test 4: a - b + c
{
  CalcCmd input[] = {CALC_CMD_LOAD, CALC_CMD_SUB, CALC_CMD_LOAD, CALC_CMD_ADD, CALC_CMD_LOAD};
  CalcCmd expected[] = {CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_SUB, CALC_CMD_LOAD, CALC_CMD_ADD};

  NAME("a - b + c");
  auto result = calc_gen_rpn(make_cmds(input, 5));
  TEST_CASE("Match", cmds_equal(result.ok, make_cmds(expected, 5)));
}

// Test 5: a * (b + c) / d
{
  CalcCmd input[] = {CALC_CMD_LOAD, CALC_CMD_MUL,     CALC_CMD_L_BRACE, CALC_CMD_LOAD, CALC_CMD_LOAD,
                     CALC_CMD_ADD,  CALC_CMD_R_BRACE, CALC_CMD_DIV,     CALC_CMD_LOAD};
  CalcCmd expected[] = {CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_ADD, CALC_CMD_MUL, CALC_CMD_LOAD, CALC_CMD_DIV};

  NAME("a * (b + c) / d");
  auto result = calc_gen_rpn(make_cmds(input, 9));
  TEST_CASE("Match", cmds_equal(result.ok, make_cmds(expected, 7)));
}

// Test 6: Unary negation -a + b
{
  CalcCmd input[] = {CALC_CMD_NEG, CALC_CMD_LOAD, CALC_CMD_ADD, CALC_CMD_LOAD};
  CalcCmd expected[] = {CALC_CMD_LOAD, CALC_CMD_NEG, CALC_CMD_LOAD, CALC_CMD_ADD};

  NAME("-a + b");
  auto result = calc_gen_rpn(make_cmds(input, 4));
  TEST_CASE("Match", cmds_equal(result.ok, make_cmds(expected, 4)));
}

// Test 7: Nested parentheses ((a + b) * (c - d))
{
  CalcCmd input[] = {CALC_CMD_L_BRACE, CALC_CMD_L_BRACE, CALC_CMD_LOAD, CALC_CMD_LOAD,    CALC_CMD_ADD, CALC_CMD_R_BRACE, CALC_CMD_L_BRACE,
                     CALC_CMD_LOAD,    CALC_CMD_LOAD,    CALC_CMD_SUB,  CALC_CMD_R_BRACE, CALC_CMD_MUL, CALC_CMD_R_BRACE};
  CalcCmd expected[] = {CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_ADD, CALC_CMD_LOAD, CALC_CMD_LOAD, CALC_CMD_SUB, CALC_CMD_MUL};

  NAME("((a + b) * (c - d))");
  auto result = calc_gen_rpn(make_cmds(input, 13));
  TEST_CASE("Match", cmds_equal(result.ok, make_cmds(expected, 7)));
}

// Test 8: Redundant parens: (((a)))
{
  CalcCmd input[] = {CALC_CMD_L_BRACE, CALC_CMD_L_BRACE, CALC_CMD_L_BRACE, CALC_CMD_LOAD, CALC_CMD_R_BRACE, CALC_CMD_R_BRACE, CALC_CMD_R_BRACE};
  CalcCmd expected[] = {CALC_CMD_LOAD};

  NAME("(((a)))");
  auto result = calc_gen_rpn(make_cmds(input, 7));
  TEST_CASE("Match", cmds_equal(result.ok, make_cmds(expected, 1)));
}

// Test 9: Right-associativity test: -a - -b
{
  CalcCmd input[] = {
      CALC_CMD_NEG, CALC_CMD_LOAD,              // -a
      CALC_CMD_SUB, CALC_CMD_NEG, CALC_CMD_LOAD // -b
  };

  CalcCmd expected[] = {CALC_CMD_LOAD, CALC_CMD_NEG, CALC_CMD_LOAD, CALC_CMD_NEG, CALC_CMD_SUB};

  NAME("-a - -b");
  auto result = calc_gen_rpn(make_cmds(input, 5));
  TEST_CASE("Match", cmds_equal(result.ok, make_cmds(expected, 5)));
}

END_TEST();
