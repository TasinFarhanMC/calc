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

TEST_CASE("Floating: Edge cases and pointer position") {
  CalcError err;
  const char *endptr = nullptr;

  SECTION("Stops at non-digit") {
    auto num = parse_num("123abc", err, &endptr);
    REQUIRE(err == CALC_ERROR_NONE);
    REQUIRE(*endptr == 'a');
    REQUIRE_NUM_EQ(num, 123);
  }

  SECTION("Zero") {
    auto num = parse_num("0", err);
    REQUIRE(err == CALC_ERROR_NONE);
    REQUIRE_NUM_EQ(num, 0);
  }

  SECTION("Large integer") {
    auto num = parse_num("32767", err);
    REQUIRE(err == CALC_ERROR_NONE);
    REQUIRE_NUM_EQ(num, 32767);
  }
}
