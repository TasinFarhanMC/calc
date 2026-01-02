#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
using namespace Catch;

#define CALC_STATIC_IMPLEMENTATION
#define CALC_INT long
#include "calc.h"

// Helper: parse number with stop boundary, default = -1 (parse all)
static CalcNum parse_num(const char *str, const char **endptr = nullptr) {
  const char *p = str;
  CalcNum num = calc_ascii_to_num((const CalcByte *)p, (const CalcByte **)&p, (const CalcByte *)-1);
  if (endptr) *endptr = p;
  return num;
}

#define REQUIRE_NUM_EQ(actual, expected)                                                                                                               \
  do {                                                                                                                                                 \
    CalcUint _actual = actual;                                                                                                                         \
    CalcUint _expected = (CalcUint)std::lround((double)(expected) * CALC_CAST_NUM(1));                                                                 \
    CAPTURE(_actual, _expected);                                                                                                                       \
    REQUIRE(_actual - _expected <= 1);                                                                                                                 \
  } while (0)

TEST_CASE("Fixed: Integer parsing") {
  SECTION("Positive integer") { REQUIRE_NUM_EQ(parse_num("123"), 123); }
  SECTION("Negative integer") { REQUIRE_NUM_EQ(parse_num("-456"), -456); }
  SECTION("Leading plus sign") { REQUIRE_NUM_EQ(parse_num("+42"), 42); }
  SECTION("Leading zeros") { REQUIRE_NUM_EQ(parse_num("0007"), 7); }
}

TEST_CASE("Fixed: Fractional parsing") {
  SECTION("Simple fraction") { REQUIRE_NUM_EQ(parse_num("12.5"), 12.5); }
  SECTION("Multiple fractional digits") { REQUIRE_NUM_EQ(parse_num("3.141592653589"), 3.141592653589); }
  SECTION("No integer part") { REQUIRE_NUM_EQ(parse_num(".25"), 0.25); }
  SECTION("Trailing zeros in fraction") { REQUIRE_NUM_EQ(parse_num("5.2500"), 5.25); }
}

TEST_CASE("Fixed: Exponent parsing") {
  SECTION("Positive exponent") { REQUIRE_NUM_EQ(parse_num("1e3"), 1000); }
  SECTION("Negative exponent") { REQUIRE_NUM_EQ(parse_num("5e-2"), 0.05); }
  SECTION("Fraction with exponent") { REQUIRE_NUM_EQ(parse_num("1.5e2"), 150); }
  SECTION("Negative fraction with negative exponent") { REQUIRE_NUM_EQ(parse_num("-3.2e-1"), -0.32); }
}

TEST_CASE("Fixed: Edge cases and pointer position") {
  const char *endptr = nullptr;

  SECTION("Stops at non-digit") {
    CalcNum num = calc_ascii_to_num((const CalcByte *)"123abc", (const CalcByte **)&endptr, (const CalcByte *)-1);
    REQUIRE(*endptr == 'a');
    REQUIRE_NUM_EQ(num, 123);
  }

  SECTION("Zero") { REQUIRE_NUM_EQ(parse_num("0"), 0); }
  SECTION("Large integer") { REQUIRE_NUM_EQ(parse_num("32767"), 32767); }
}
