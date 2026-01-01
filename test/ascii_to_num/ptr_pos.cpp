#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
using namespace Catch;

#define CALC_STATIC_IMPLEMENTATION
#define CALC_FLOAT float
#include "calc.h"

static CalcNum parse_num(const char *str, const char **endptr = nullptr) {
  const char *p = str;
  CalcNum num = calc_ascii_to_num((const CalcByte *)p, (const CalcByte **)&p);
  if (endptr) *endptr = p;
  return num;
}

#define REQUIRE_NUM_EQ(actual, expected) REQUIRE(actual == Approx(expected))

TEST_CASE("Floating: Edge cases and end_ptr behavior") {
  const char *endptr = nullptr;

  SECTION("Stops at non-digit after integer") {
    auto num = parse_num("123abc", &endptr);
    REQUIRE(*endptr == 'a');
    REQUIRE_NUM_EQ(num, 123);
  }

  SECTION("Stops at non-digit after fraction") {
    auto num = parse_num("45.67xyz", &endptr);
    REQUIRE(*endptr == 'x');
    REQUIRE_NUM_EQ(num, 45.67f);
  }

  SECTION("Stops at non-digit after exponent") {
    auto num = parse_num("1.23e2!", &endptr);
    REQUIRE(*endptr == '!');
    REQUIRE_NUM_EQ(num, 123.0f);
  }

  SECTION("Stops at end of string") {
    auto num = parse_num("99.99", &endptr);
    REQUIRE(*endptr == '\0');
    REQUIRE_NUM_EQ(num, 99.99f);
  }

  SECTION("Empty string") {
    auto num = parse_num("", &endptr);
    REQUIRE(*endptr == '\0');
    REQUIRE_NUM_EQ(num, 0.0f);
  }

  SECTION("Leading whitespace skipped") {
    auto num = parse_num("42", &endptr);
    REQUIRE(*endptr == '\0');
    REQUIRE_NUM_EQ(num, 42.0f);
  }

  SECTION("Fraction without integer part") {
    auto num = parse_num(".75xyz", &endptr);
    REQUIRE(*endptr == 'x');
    REQUIRE_NUM_EQ(num, 0.75f);
  }

  SECTION("Negative number") {
    auto num = parse_num("-12.5abc", &endptr);
    REQUIRE(*endptr == 'a');
    REQUIRE_NUM_EQ(num, -12.5f);
  }

  SECTION("Positive sign") {
    auto num = parse_num("+3.14xyz", &endptr);
    REQUIRE(*endptr == 'x');
    REQUIRE_NUM_EQ(num, 3.14f);
  }

  SECTION("Exponent with negative power") {
    auto num = parse_num("1.5e-2!", &endptr);
    REQUIRE(*endptr == '!');
    REQUIRE_NUM_EQ(num, 0.015f);
  }

  SECTION("Exponent with positive power") {
    auto num = parse_num("2e3", &endptr);
    REQUIRE(*endptr == '\0');
    REQUIRE_NUM_EQ(num, 2000.0f);
  }

  SECTION("Stops at first invalid character") {
    auto num = parse_num("123.45.67", &endptr);
    REQUIRE(*endptr == '.'); // second decimal is invalid
    REQUIRE_NUM_EQ(num, 123.45f);
  }
}
