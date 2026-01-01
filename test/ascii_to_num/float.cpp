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

TEST_CASE("Floating: Integer parsing") {
  SECTION("Positive integer") {
    auto num = parse_num("123");
    REQUIRE_NUM_EQ(num, 123);
  }

  SECTION("Negative integer") {
    auto num = parse_num("-456");
    REQUIRE_NUM_EQ(num, -456);
  }

  SECTION("Leading plus sign") {
    auto num = parse_num("+42");
    REQUIRE_NUM_EQ(num, 42);
  }

  SECTION("Leading zeros") {
    auto num = parse_num("0007");
    REQUIRE_NUM_EQ(num, 7);
  }
}

TEST_CASE("Floating: Fractional parsing") {
  SECTION("Simple fraction") {
    auto num = parse_num("12.5");
    REQUIRE_NUM_EQ(num, 12.5f);
  }

  SECTION("Multiple fractional digits") {
    auto num = parse_num("3.141592653589");
    REQUIRE_NUM_EQ(num, 3.141592653589f);
  }

  SECTION("No integer part") {
    auto num = parse_num(".25");
    REQUIRE_NUM_EQ(num, 0.25f);
  }

  SECTION("Trailing zeros in fraction") {
    auto num = parse_num("5.2500");
    REQUIRE_NUM_EQ(num, 5.25f);
  }
}

TEST_CASE("Floating: Exponent parsing") {
  SECTION("Positive exponent") {
    auto num = parse_num("1e3");
    REQUIRE_NUM_EQ(num, 1000.0f);
  }

  SECTION("Negative exponent") {
    auto num = parse_num("5e-2");
    REQUIRE_NUM_EQ(num, 0.05f);
  }

  SECTION("Fraction with exponent") {
    auto num = parse_num("1.5e2");
    REQUIRE_NUM_EQ(num, 150.0f);
  }

  SECTION("Negative fraction with negative exponent") {
    auto num = parse_num("-3.2e-1");
    REQUIRE_NUM_EQ(num, -0.32f);
  }
}
