#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
using namespace Catch;

#define CALC_STATIC_IMPLEMENTATION
#define CALC_FLOAT float
#include "calc.h"

// Helper: parse number with stop boundary (default = parse all)
static CalcNum parse_num(const char *str, const char **endptr = nullptr) {
  const char *p = str;
  CalcNum num = calc_ascii_to_num((const CalcByte *)p, (const CalcByte **)&p, (const CalcByte *)-1);
  if (endptr) *endptr = p;
  return num;
}

#define REQUIRE_NUM_EQ(actual, expected) REQUIRE(actual == Approx(expected))

TEST_CASE("Floating: Integer parsing") {
  SECTION("Positive integer") { REQUIRE_NUM_EQ(parse_num("123"), 123.0f); }
  SECTION("Negative integer") { REQUIRE_NUM_EQ(parse_num("-456"), -456.0f); }
  SECTION("Leading plus sign") { REQUIRE_NUM_EQ(parse_num("+42"), 42.0f); }
  SECTION("Leading zeros") { REQUIRE_NUM_EQ(parse_num("0007"), 7.0f); }
}

TEST_CASE("Floating: Fractional parsing") {
  SECTION("Simple fraction") { REQUIRE_NUM_EQ(parse_num("12.5"), 12.5f); }
  SECTION("Multiple fractional digits") { REQUIRE_NUM_EQ(parse_num("3.141592653589"), 3.141592653589f); }
  SECTION("No integer part") { REQUIRE_NUM_EQ(parse_num(".25"), 0.25f); }
  SECTION("Trailing zeros in fraction") { REQUIRE_NUM_EQ(parse_num("5.2500"), 5.25f); }
}

TEST_CASE("Floating: Exponent parsing") {
  SECTION("Positive exponent") { REQUIRE_NUM_EQ(parse_num("1e3"), 1000.0f); }
  SECTION("Negative exponent") { REQUIRE_NUM_EQ(parse_num("5e-2"), 0.05f); }
  SECTION("Fraction with exponent") { REQUIRE_NUM_EQ(parse_num("1.5e2"), 150.0f); }
  SECTION("Negative fraction with negative exponent") { REQUIRE_NUM_EQ(parse_num("-3.2e-1"), -0.32f); }
}

TEST_CASE("Floating: Edge cases and partial parsing") {
  const char *endptr = nullptr;

  SECTION("Stops at non-digit") {
    CalcNum num = calc_ascii_to_num((const CalcByte *)"123abc", (const CalcByte **)&endptr, (const CalcByte *)-1);
    REQUIRE(*endptr == 'a');
    REQUIRE_NUM_EQ(num, 123.0f);
  }

  SECTION("Stops at decimal with trailing non-digit") {
    CalcNum num = calc_ascii_to_num((const CalcByte *)"12.34xyz", (const CalcByte **)&endptr, (const CalcByte *)-1);
    REQUIRE(*endptr == 'x');
    REQUIRE_NUM_EQ(num, 12.34f);
  }

  SECTION("Zero") { REQUIRE_NUM_EQ(parse_num("0"), 0.0f); }

  SECTION("Leading zeros in integer") {
    CalcNum num = calc_ascii_to_num((const CalcByte *)"0000123.45", (const CalcByte **)&endptr, (const CalcByte *)-1);
    REQUIRE_NUM_EQ(num, 123.45f);
  }

  SECTION("Large integer") {
    CalcNum num = calc_ascii_to_num((const CalcByte *)"32767", (const CalcByte **)&endptr, (const CalcByte *)-1);
    REQUIRE_NUM_EQ(num, 32767.0f);
  }

  SECTION("Partial parsing with stop pointer") {
    const char *stop = "12345" + 3; // stop after first 3 chars
    CalcNum num = calc_ascii_to_num((const CalcByte *)"12345", nullptr, (const CalcByte *)stop);
    REQUIRE_NUM_EQ(num, 123.0f); // only parses "123"
  }
}
