#define CATCH_CONFIG_MAIN

#include <calc.h>
#include <catch2/catch_all.hpp>
#include <cmath>
#include <cstring>
#include <iostream>

using namespace Catch;

// Keep your dump functions (optional, used for debugging)
static void dump_bufs(const CalcBufs &bufs) {
  std::cerr << "Nums (" << (int)bufs.nums.size << "): ";
  for (int i = 0; i < bufs.nums.size; ++i) std::cerr << bufs.nums.data[i].val << " ";
  std::cerr << "\nCmds (" << (int)bufs.cmds.size << "): ";
  for (int i = 0; i < bufs.cmds.size; ++i) std::cerr << bufs.cmds.data[i] << " ";
  std::cerr << std::endl;
}

static void dump_bufs(const CalcCmds &bufs) {
  std::cerr << "Cmds (" << (int)bufs.size << "): ";
  for (int i = 0; i < bufs.size; ++i) std::cerr << bufs.data[i] << " ";
  std::cerr << std::endl;
}

static void dump_bufs(const CalcNum &bufs) { std::cerr << "Res :" << bufs.val << std::endl; }

#ifdef CALC_NUM_FLOAT

TEST_CASE("Parse integer 123", "[calc_parse_ascii]") {
  const char *expr = "123";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_NONE);
  REQUIRE(result.ok.nums.size == 1);
  REQUIRE(result.ok.nums.data[0].val == 123);
  REQUIRE(result.ok.cmds.size == 1);
  REQUIRE(result.ok.cmds.data[0] == CALC_CMD_LOAD);
}

TEST_CASE("Parse negative number -5", "[calc_parse_ascii]") {
  const char *expr = "-5";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_NONE);
  REQUIRE(result.ok.cmds.size == 2);
  REQUIRE(result.ok.cmds.data[0] == CALC_CMD_NEG);
  REQUIRE(result.ok.cmds.data[1] == CALC_CMD_LOAD);
  REQUIRE(result.ok.nums.data[0].val == 5);
}

TEST_CASE("Parse simple addition 5+7", "[calc_parse_ascii]") {
  const char *expr = "5+7";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_NONE);
  REQUIRE(result.ok.nums.size == 2);
  REQUIRE(result.ok.cmds.size == 3);
  REQUIRE(result.ok.cmds.data[0] == CALC_CMD_LOAD);
  REQUIRE(result.ok.cmds.data[1] == CALC_CMD_ADD);
  REQUIRE(result.ok.cmds.data[2] == CALC_CMD_LOAD);
  REQUIRE(result.ok.nums.data[0].val == 5);
  REQUIRE(result.ok.nums.data[1].val == 7);
}

TEST_CASE("Parse expression with parentheses (2+3)*4", "[calc_parse_ascii]") {
  const char *expr = "(2+3)*4";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_NONE);
  REQUIRE(result.ok.nums.size == 3);
  REQUIRE(result.ok.cmds.size == 7);
  REQUIRE(result.ok.cmds.data[0] == CALC_CMD_L_BRACE);
  REQUIRE(result.ok.cmds.data[1] == CALC_CMD_LOAD);
  REQUIRE(result.ok.cmds.data[2] == CALC_CMD_ADD);
  REQUIRE(result.ok.cmds.data[3] == CALC_CMD_LOAD);
  REQUIRE(result.ok.cmds.data[4] == CALC_CMD_R_BRACE);
  REQUIRE(result.ok.cmds.data[5] == CALC_CMD_MUL);
}

TEST_CASE("Parse negative plus positive -5+2", "[calc_parse_ascii]") {
  const char *expr = "-5+2";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_NONE);
  REQUIRE(result.ok.nums.size == 2);
  REQUIRE(result.ok.cmds.size == 4);
  REQUIRE(result.ok.cmds.data[0] == CALC_CMD_NEG);
  REQUIRE(result.ok.cmds.data[1] == CALC_CMD_LOAD);
  REQUIRE(result.ok.cmds.data[2] == CALC_CMD_ADD);
  REQUIRE(result.ok.cmds.data[3] == CALC_CMD_LOAD);
}

TEST_CASE("Parse expression with unknown char", "[calc_parse_ascii]") {
  const char *expr = "5+&7";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_UNKNOWN_CHAR);
}

TEST_CASE("Parse invalid fractional .5", "[calc_parse_ascii]") {
  const char *expr = ".5";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_INV_FRAC);
}

TEST_CASE("Parse scientific notation 1.23e2", "[calc_parse_ascii]") {
  const char *expr = "1.23e2";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_NONE);
  REQUIRE(result.ok.nums.size == 1);
  REQUIRE(result.ok.nums.data[0].val == Approx(123.0f));
}

TEST_CASE("Parse scientific notation overflow 1e9000", "[calc_parse_ascii]") {
  const char *expr = "1e9000";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_NUM_OVERFLOW);
}

TEST_CASE("Parse small scientific notation 1.23e-2", "[calc_parse_ascii]") {
  const char *expr = "1.23e-2";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_NONE);
  REQUIRE(result.ok.nums.size == 1);
  REQUIRE(result.ok.nums.data[0].val == Approx(0.0123).margin(0.0001));
}

TEST_CASE("Parse zero 0", "[calc_parse_ascii]") {
  const char *expr = "0";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_NONE);
  REQUIRE(result.ok.nums.data[0].val == 0);
}

TEST_CASE("Parse negative zero -0", "[calc_parse_ascii]") {
  const char *expr = "-0";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_NONE);
  REQUIRE(result.ok.nums.data[0].val == 0);
}

TEST_CASE("Parse partial input 123abc with length 3", "[calc_parse_ascii]") {
  const char *expr = "123abc";
  auto result = calc_parse_ascii(expr, 3); // only parse "123"
  REQUIRE(result.err == CALC_ERR_NONE);
  REQUIRE(result.ok.nums.data[0].val == 123);
  REQUIRE(result.ok.nums.size == 1);
}

TEST_CASE("Parse tiny number 1e-40", "[calc_parse_ascii]") {
  const char *expr = "1e-40";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_NONE);
  REQUIRE(std::fabs(result.ok.nums.data[0].val) < 1e-30f);
}

TEST_CASE("Parse nan string", "[calc_parse_ascii]") {
  const char *expr = "nan";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_UNKNOWN_CHAR);
}

TEST_CASE("Parse +123", "[calc_parse_ascii]") {
  const char *expr = "+123";
  auto result = calc_parse_ascii(expr, strlen(expr));
  REQUIRE(result.err == CALC_ERR_NONE);
  REQUIRE(result.ok.nums.data[0].val == 123);
}

#else

TEST_CASE("Placeholder test when CALC_NUM_FLOAT not defined") { SUCCEED("No tests run"); }

#endif // CALC_NUM_FLOAT
