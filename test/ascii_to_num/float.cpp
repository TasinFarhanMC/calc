#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
using namespace Catch;

#define CALC_STATIC_IMPLEMENTATION
#define CALC_FLOAT float
#include "calc.h"

static CalcNum parse_num(const char *str, CalcError &err_out, const char **endptr = nullptr) {
  const char *p = str;
  CalcNum num = calc_ascii_to_num(p, &p, &err_out);
  if (endptr) *endptr = p;
  return num;
}

#define REQUIRE_NUM_EQ(actual, expected) REQUIRE(actual == Approx(expected))

TEST_CASE("Floating: Integer parsing") {
  CalcError err;

  SECTION("Positive integer") {
    auto num = parse_num("123", err);
    REQUIRE(err == CALC_ERROR_NONE);
    REQUIRE_NUM_EQ(num, 123);
  }

  SECTION("Negative integer") {
    auto num = parse_num("-456", err);
    REQUIRE(err == CALC_ERROR_NONE);
    REQUIRE_NUM_EQ(num, -456);
  }

  SECTION("Leading plus sign") {
    auto num = parse_num("+42", err);
    REQUIRE(err == CALC_ERROR_NONE);
    REQUIRE_NUM_EQ(num, 42);
  }

  SECTION("Leading zeros") {
    auto num = parse_num("0007", err);
    REQUIRE(err == CALC_ERROR_NONE);
    REQUIRE_NUM_EQ(num, 7);
  }
}

TEST_CASE("Floating: Fractional parsing") {
  CalcError err;

  SECTION("Simple fraction") {
    auto num = parse_num("12.5", err);
    REQUIRE(err == CALC_ERROR_NONE);
    REQUIRE_NUM_EQ(num, 12.5);
  }

  SECTION("Multiple fractional digits") {
    auto num = parse_num("3.1415", err);
    REQUIRE(err == CALC_ERROR_NONE);
    REQUIRE_NUM_EQ(num, 3.1415);
  }

  SECTION("No integer part") {
    auto num = parse_num(".25", err);
    REQUIRE(err == CALC_ERROR_NONE);
    REQUIRE_NUM_EQ(num, 0.25);
  }

  SECTION("Trailing zeros in fraction") {
    auto num = parse_num("5.2500", err);
    REQUIRE(err == CALC_ERROR_NONE);
    REQUIRE_NUM_EQ(num, 5.25);
  }
}

// TEST_CASE("Floating: Exponent parsing") {
//   CalcErr err;
//
//   SECTION("Positive exponent") {
//     auto num = parse_num("1e3", err);
//     REQUIRE(err == CALC_ERR_NONE);
//     REQUIRE_NUM_EQ(num, 1000);
//   }
//
//   SECTION("Negative exponent") {
//     auto num = parse_num("5e-2", err);
//     REQUIRE(err == CALC_ERR_NONE);
//     REQUIRE_NUM_EQ(num, 0.05);
//   }
//
//   SECTION("Fraction with exponent") {
//     auto num = parse_num("1.5e2", err);
//     REQUIRE(err == CALC_ERR_NONE);
//     REQUIRE_NUM_EQ(num, 150);
//   }
//
//   SECTION("Negative fraction with negative exponent") {
//     auto num = parse_num("-3.2e-1", err);
//     REQUIRE(err == CALC_ERR_NONE);
//     REQUIRE_NUM_EQ(num, -0.32);
//   }
// }
