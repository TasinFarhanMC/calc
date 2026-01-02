#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
using namespace Catch;

#define CALC_STATIC_IMPLEMENTATION
#define CALC_FLOAT float
#include "calc.h"

TEST_CASE("calc_convert_rpn basic infix to postfix") {
  CalcCmdData rpn;
  CalcCmd rpn_buffer[16];

  rpn.data = rpn_buffer;
  rpn.capacity = 16;

  SECTION("1 + 2") {
    rpn.length = 0;

    CalcCmdData infix;
    CalcCmd infix_buf[16] = {CALC_CMD_LOAD, CALC_CMD_ADD, CALC_CMD_LOAD};

    infix.data = infix_buf;
    infix.length = 3;
    infix.capacity = 16;

    CalcError err = calc_convert_rpn(infix, &rpn);
    REQUIRE(err == CALC_ERROR_NONE);

    REQUIRE(rpn.length == 3);
    REQUIRE(rpn.data[0] == CALC_CMD_LOAD);
    REQUIRE(rpn.data[1] == CALC_CMD_LOAD);
    REQUIRE(rpn.data[2] == CALC_CMD_ADD);
  }

  SECTION("1 + 2 * 3") {
    rpn.length = 0;

    CalcCmdData infix;
    CalcCmd infix_buf[16] = {CALC_CMD_LOAD, CALC_CMD_ADD, CALC_CMD_LOAD, CALC_CMD_MUL, CALC_CMD_LOAD};

    infix.data = infix_buf;
    infix.length = 5;
    infix.capacity = 16;

    CalcError err = calc_convert_rpn(infix, &rpn);
    REQUIRE(err == CALC_ERROR_NONE);

    REQUIRE(rpn.length == 5);
    // Postfix: 1 2 3 * +
    REQUIRE(rpn.data[0] == CALC_CMD_LOAD);
    REQUIRE(rpn.data[1] == CALC_CMD_LOAD);
    REQUIRE(rpn.data[2] == CALC_CMD_LOAD);
    REQUIRE(rpn.data[3] == CALC_CMD_MUL);
    REQUIRE(rpn.data[4] == CALC_CMD_ADD);
  }

  SECTION("(1 + 2) * 3") {
    rpn.length = 0;

    CalcCmdData infix;
    CalcCmd infix_buf[16] = {CALC_CMD_L_BRACE, CALC_CMD_LOAD, CALC_CMD_ADD, CALC_CMD_LOAD, CALC_CMD_R_BRACE, CALC_CMD_MUL, CALC_CMD_LOAD};

    infix.data = infix_buf;
    infix.length = 7;
    infix.capacity = 16;

    CalcError err = calc_convert_rpn(infix, &rpn);
    REQUIRE(err == CALC_ERROR_NONE);

    REQUIRE(rpn.length == 5);
    // Postfix: 1 2 + 3 *
    REQUIRE(rpn.data[0] == CALC_CMD_LOAD);
    REQUIRE(rpn.data[1] == CALC_CMD_LOAD);
    REQUIRE(rpn.data[2] == CALC_CMD_ADD);
    REQUIRE(rpn.data[3] == CALC_CMD_LOAD);
    REQUIRE(rpn.data[4] == CALC_CMD_MUL);
  }

  SECTION("Unary minus: -1 + 2") {
    rpn.length = 0;

    CalcCmdData infix;
    CalcCmd infix_buf[16] = {CALC_CMD_NEG, CALC_CMD_LOAD, CALC_CMD_ADD, CALC_CMD_LOAD};

    infix.data = infix_buf;
    infix.length = 4;
    infix.capacity = 16;

    CalcError err = calc_convert_rpn(infix, &rpn);
    REQUIRE(err == CALC_ERROR_NONE);

    REQUIRE(rpn.length == 4);
    // Postfix: 1 neg 2 +
    REQUIRE(rpn.data[0] == CALC_CMD_LOAD);
    REQUIRE(rpn.data[1] == CALC_CMD_NEG);
    REQUIRE(rpn.data[2] == CALC_CMD_LOAD);
    REQUIRE(rpn.data[3] == CALC_CMD_ADD);
  }

  SECTION("Complex: 1 + (2 - 3) * 4") {
    rpn.length = 0;

    CalcCmdData infix;
    CalcCmd infix_buf[16] = {CALC_CMD_LOAD, CALC_CMD_ADD,     CALC_CMD_L_BRACE, CALC_CMD_LOAD, CALC_CMD_SUB,
                             CALC_CMD_LOAD, CALC_CMD_R_BRACE, CALC_CMD_MUL,     CALC_CMD_LOAD};

    infix.data = infix_buf;
    infix.length = 9;
    infix.capacity = 16;

    CalcError err = calc_convert_rpn(infix, &rpn);
    REQUIRE(err == CALC_ERROR_NONE);

    REQUIRE(rpn.length == 7);
    // Postfix: 1 2 3 - 4 * +
    REQUIRE(rpn.data[0] == CALC_CMD_LOAD);
    REQUIRE(rpn.data[1] == CALC_CMD_LOAD);
    REQUIRE(rpn.data[2] == CALC_CMD_LOAD);
    REQUIRE(rpn.data[3] == CALC_CMD_SUB);
    REQUIRE(rpn.data[4] == CALC_CMD_LOAD);
    REQUIRE(rpn.data[5] == CALC_CMD_MUL);
    REQUIRE(rpn.data[6] == CALC_CMD_ADD);
  }
}
