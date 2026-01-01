#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
using namespace Catch;

#define CALC_STATIC_IMPLEMENTATION
#define CALC_INT long
#include "calc.h"

static CalcNum parse_num(const char *str, const char **endptr = nullptr) {
  const char *p = str;
  CalcNum num = calc_ascii_to_num((const CalcByte *)p, (const CalcByte **)&p);
  if (endptr) *endptr = p;
  return num;
}

#define REQUIRE_NUM_EQ(actual, expected) REQUIRE((CalcUint)(actual) - (CalcUint)std::lround((double)(expected) * (1ULL << CALC_SHIFT)) <= 1)

TEST_CASE("Fixed: Integer parsing") {
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

TEST_CASE("Fixed: Fractional parsing") {
  SECTION("Simple fraction") {
    auto num = parse_num("12.5");
    REQUIRE_NUM_EQ(num, 12.5);
  }

  SECTION("Multiple fractional digits") {
    auto num = parse_num("3.141592653589");
    REQUIRE_NUM_EQ(num, 3.141592653589);
  }

  SECTION("No integer part") {
    auto num = parse_num(".25");
    REQUIRE_NUM_EQ(num, 0.25);
  }

  SECTION("Trailing zeros in fraction") {
    auto num = parse_num("5.2500");
    REQUIRE_NUM_EQ(num, 5.25);
  }
}

TEST_CASE("Fixed: Exponent parsing") {
  SECTION("Positive exponent") {
    auto num = parse_num("1e3");
    REQUIRE_NUM_EQ(num, 1000);
  }

  SECTION("Negative exponent") {
    auto num = parse_num("5e-2");
    REQUIRE_NUM_EQ(num, 0.05);
  }

  SECTION("Fraction with exponent") {
    auto num = parse_num("1.5e2");
    REQUIRE_NUM_EQ(num, 150);
  }

  SECTION("Negative fraction with negative exponent") {
    auto num = parse_num("-3.2e-1");
    REQUIRE_NUM_EQ(num, -0.32);
  }
}

TEST_CASE("Fixed: Edge cases and pointer position") {
  const char *endptr = nullptr;

  SECTION("Stops at non-digit") {
    auto num = parse_num("123abc", &endptr);
    REQUIRE(*endptr == 'a');
    REQUIRE_NUM_EQ(num, 123);
  }

  SECTION("Zero") {
    auto num = parse_num("0");
    REQUIRE_NUM_EQ(num, 0);
  }

  SECTION("Large integer") {
    auto num = parse_num("32767");
    REQUIRE_NUM_EQ(num, 32767);
  }
}
