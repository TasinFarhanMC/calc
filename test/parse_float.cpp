#ifdef CALC_NUM_FLOAT

#include "test.hpp"

START_TEST();

{
  const char *expr = "123";
  auto name = expr;
  auto result = calc_parse_ascii(expr, strlen(expr));
  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("One number", result.ok.nums.size == 1);
  TEST_CASE("Number value == 123", result.ok.nums.data[0].val == 123);
  TEST_CASE("One load cmd", result.ok.cmds.size == 1);
  TEST_CASE("Cmd is LOAD", result.ok.cmds.data[0] == CALC_CMD_LOAD);
}

{
  const char *expr = "-5";
  auto name = expr;
  auto result = calc_parse_ascii(expr, strlen(expr));
  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Two cmds", result.ok.cmds.size == 2);
  TEST_CASE("Cmd0 NEG", result.ok.cmds.data[0] == CALC_CMD_NEG);
  TEST_CASE("Cmd1 LOAD", result.ok.cmds.data[1] == CALC_CMD_LOAD);
  TEST_CASE("Number value == 5", result.ok.nums.data[0].val == 5);
}

{
  const char *expr = "5+7";
  auto name = expr;
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
  const char *expr = "(2+3)*4";
  auto name = expr;
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
  const char *expr = "-5+2";
  auto name = expr;
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
  const char *expr = "5+&7";
  auto name = expr;
  auto result = calc_parse_ascii(expr, strlen(expr));
  TEST_CASE("Error unknown char", result.err == CALC_ERR_UNKNOWN_CHAR);
}

{
  const char *expr = ".5";
  auto name = expr;
  auto result = calc_parse_ascii(expr, strlen(expr));
  TEST_CASE("Error invalid frac", result.err == CALC_ERR_INVALID_FRAC);
}

{
  const char *expr = "1.23e2";
  auto name = expr;
  auto result = calc_parse_ascii(expr, strlen(expr));
  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("One number", result.ok.nums.size == 1);
  TEST_CASE("Value ~ 123", fabs(result.ok.nums.data[0].val - 123) < 0.01f);
}

{
  const char *expr = "1e9000";
  auto name = expr;
  auto result = calc_parse_ascii(expr, strlen(expr));
  TEST_CASE("Error overflow", result.err == CALC_ERR_NUM_OVERFLOW);
}

{
  const char *expr = "1.23e-2";
  auto name = expr;
  auto result = calc_parse_ascii(expr, strlen(expr));
  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Value ~ 0.0123", fabs(result.ok.nums.data[0].val - 0.0123) < 0.0001f);
}

{
  const char *expr = "0";
  auto name = expr;
  auto result = calc_parse_ascii(expr, strlen(expr));
  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Value == 0", result.ok.nums.data[0].val == 0);
}

{
  const char *expr = "-0";
  auto name = expr;
  auto result = calc_parse_ascii(expr, strlen(expr));
  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Value == 0", result.ok.nums.data[0].val == 0);
}

{
  const char *expr = "123abc";
  auto name = expr;
  auto result = calc_parse_ascii(expr, 3); // only parse "123"
  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Value == 123", result.ok.nums.data[0].val == 123);
  TEST_CASE("Only one number parsed", result.ok.nums.size == 1);
}

{
  const char *expr = "1e-40";
  auto name = expr;
  auto result = calc_parse_ascii(expr, strlen(expr));
  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Value near zero", fabs(result.ok.nums.data[0].val) < 1e-30f);
}

{
  const char *expr = "nan";
  auto name = expr;
  auto result = calc_parse_ascii(expr, strlen(expr));
  TEST_CASE("Unknown char error", result.err == CALC_ERR_UNKNOWN_CHAR);
}

{
  const char *expr = "+123";
  auto name = expr;
  auto result = calc_parse_ascii(expr, strlen(expr));
  TEST_CASE("No error", result.err == CALC_ERR_NONE);
  TEST_CASE("Value == 123", result.ok.nums.data[0].val == 123);
}

END_TEST();

#else
int main() {}
#endif // CALC_NUM_FLOAT
