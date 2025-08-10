#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
using namespace Catch;

#define CALC_STATIC_IMPLEMENTATION
#define CALC_INT long
#include "calc.h"

static CalcNum parse_num(const char *str, CalcErr &err_out, const char **endptr = nullptr) {
  const char *p = str;
  CalcNum num = calc_ascii_to_num(p, &p, &err_out);
  if (endptr) *endptr = p;
  return num;
}

TEST_CASE("Error handling") {
  CalcErr err;

  SECTION("Missing exponent digits") {
    parse_num("1e", err);
    REQUIRE(err == CALC_ERR_INVALID_SYNTAX);
  }
}
