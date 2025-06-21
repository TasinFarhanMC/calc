#ifdef CALC_NUM_FLOAT

#include "cmath"
#include <cstring>
#include <iostream>

#include <calc.h>

// Helper to dump buffers for debug on failure
void dump_bufs(const CalcBufs &bufs) {
  std::cerr << "Nums (" << (int)bufs.nums.size << "): ";
  for (int i = 0; i < bufs.nums.size; i++) { std::cerr << bufs.nums.data[i].val << " "; }
  std::cerr << "\nCmds (" << (int)bufs.cmds.size << "): ";
  for (int i = 0; i < bufs.cmds.size; i++) { std::cerr << bufs.cmds.data[i] << " "; }
  std::cerr << std::endl;
}

#define TEST_CASE(desc, cond)                                                                                                                          \
  do {                                                                                                                                                 \
    if (!(cond)) {                                                                                                                                     \
      std::cerr << "Test failed: " << (desc) << std::endl;                                                                                             \
      dump_bufs(result.ok);                                                                                                                            \
      return 1;                                                                                                                                        \
    }                                                                                                                                                  \
  } while (0)

int main() {
  {
    // Test 1: Simple number
    const char *expr = "123";
    auto result = calc_parse_ascii(expr, strlen(expr));
    TEST_CASE("No error", result.err == CALC_ERR_NONE);
    TEST_CASE("One number", result.ok.nums.size == 1);
    TEST_CASE("Number value == 123", result.ok.nums.data[0].val == 123);
    TEST_CASE("One load cmd", result.ok.cmds.size == 1);
    TEST_CASE("Cmd is LOAD", result.ok.cmds.data[0] == CALC_CMD_LOAD);
  }

  {
    // Test 2: Unary negation -5
    const char *expr = "-5";
    auto result = calc_parse_ascii(expr, strlen(expr));
    TEST_CASE("No error", result.err == CALC_ERR_NONE);
    TEST_CASE("Two cmds", result.ok.cmds.size == 2);
    TEST_CASE("Cmd0 NEG", result.ok.cmds.data[0] == CALC_CMD_NEG);
    TEST_CASE("Cmd1 LOAD", result.ok.cmds.data[1] == CALC_CMD_LOAD);
    TEST_CASE("Number value == 5", result.ok.nums.data[0].val == 5);
  }

  {
    // Test 3: Simple addition 5 + 7
    const char *expr = "5+7";
    auto result = calc_parse_ascii(expr, strlen(expr));
    TEST_CASE("No error", result.err == CALC_ERR_NONE);
    TEST_CASE("Two numbers", result.ok.nums.size == 2);
    TEST_CASE("Three cmds", result.ok.cmds.size == 3);
    TEST_CASE("Cmd0 LOAD", result.ok.cmds.data[0] == CALC_CMD_LOAD);
    TEST_CASE("Cmd1 ADD", result.ok.cmds.data[1] == CALC_CMD_ADD);
    TEST_CASE("Cmd2 LOAD", result.ok.cmds.data[2] == CALC_CMD_LOAD);
    TEST_CASE("Num0 == 5", result.ok.nums.data[0].val == 5);
    TEST_CASE("Num1 == 7", result.ok.nums.data[1].val == 7);
  }

  {
    // Test 4: Complex expression with parens: (2 + 3) * 4
    const char *expr = "(2+3)*4";
    auto result = calc_parse_ascii(expr, strlen(expr));
    TEST_CASE("No error", result.err == CALC_ERR_NONE);
    TEST_CASE("Three numbers", result.ok.nums.size == 3);
    TEST_CASE("Seven cmds", result.ok.cmds.size == 7);
    TEST_CASE("Cmd0 L_BRACE", result.ok.cmds.data[0] == CALC_CMD_L_BRACE);
    TEST_CASE("Cmd1 LOAD", result.ok.cmds.data[1] == CALC_CMD_LOAD);
    TEST_CASE("Cmd2 ADD", result.ok.cmds.data[2] == CALC_CMD_ADD);
    TEST_CASE("Cmd3 LOAD", result.ok.cmds.data[3] == CALC_CMD_LOAD);
    TEST_CASE("Cmd4 R_BRACE", result.ok.cmds.data[4] == CALC_CMD_R_BRACE);
    TEST_CASE("Cmd5 MUL", result.ok.cmds.data[5] == CALC_CMD_MUL);
  }

  {
    // Test 5: Unary minus combined -5 + 2
    const char *expr = "-5+2";
    auto result = calc_parse_ascii(expr, strlen(expr));
    TEST_CASE("No error", result.err == CALC_ERR_NONE);
    TEST_CASE("Two numbers", result.ok.nums.size == 2);
    TEST_CASE("Four cmds", result.ok.cmds.size == 4);
    TEST_CASE("Cmd0 NEG", result.ok.cmds.data[0] == CALC_CMD_NEG);
    TEST_CASE("Cmd1 LOAD", result.ok.cmds.data[1] == CALC_CMD_LOAD);
    TEST_CASE("Cmd2 ADD", result.ok.cmds.data[2] == CALC_CMD_ADD);
    TEST_CASE("Cmd3 LOAD", result.ok.cmds.data[3] == CALC_CMD_LOAD);
  }

  {
    // Test 6: Syntax error (invalid char)
    const char *expr = "5+&7";
    auto result = calc_parse_ascii(expr, strlen(expr));
    TEST_CASE("Error unknown char", result.err == CALC_ERR_UNKNOWN_CHAR);
  }

  {
    // Test 7: Syntax error (dot without leading digits)
    const char *expr = ".5";
    auto result = calc_parse_ascii(expr, strlen(expr));
    TEST_CASE("Error invalid frac", result.err == CALC_ERR_INVALID_FRAC);
  }

  {
    // Test 8: Number with exponent 1.23e2
    const char *expr = "1.23e2";
    auto result = calc_parse_ascii(expr, strlen(expr));
    TEST_CASE("No error", result.err == CALC_ERR_NONE);
    TEST_CASE("One number", result.ok.nums.size == 1);
    TEST_CASE("Value ~ 123", fabs(result.ok.nums.data[0].val - 123) < 0.01f);
  }

  {
    // Test 9: Overflow number (too large)
    const char *expr = "1e40";
    auto result = calc_parse_ascii(expr, strlen(expr));
    TEST_CASE("Error overflow", result.err == CALC_ERR_NUM_OVERFLOW);
  }

  std::cout << "All tests passed!\n";
  return 0;
}

#endif
