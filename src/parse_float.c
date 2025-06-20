#include <stdio.h>

#ifdef CALC_NUM_FLOAT
#include "calc.h"

#ifndef CALC_USE_STDLIB
typedef enum {
  CALC_STRTOF_OK = 0,
  CALC_STRTOF_OVERFLOW = 1,
  CALC_STRTOF_UNDERFLOW = 2,
} CalcStrtofError;

float calc_strtof(const char *s, const char **endptr, CalcStrtofError *error) {
  float result = 0.0f;
  float sign = 1.0f;

  // IEEE-754 float limits
  const float FLT_MAX_ = 3.4028235e+38f;
  const float FLT_MIN_ = 1.1754944e-38f;

  if (error) *error = CALC_STRTOF_OK;

  // Parse sign
  if (*s == '-') {
    sign = -1.0f;
    s++;
  } else if (*s == '+') {
    s++;
  }

  // Parse integer part
  while (*s >= '0' && *s <= '9') {
    float digit = (float)(*s++ - '0');
    if (result > (FLT_MAX_ - digit) / 10.0f) {
      // Overflow
      if (error) *error = CALC_STRTOF_OVERFLOW;
      if (endptr) *endptr = s;
      return sign > 0 ? FLT_MAX_ : -FLT_MAX_;
    }
    result = result * 10.0f + digit;
  }

  // Parse fractional part
  if (*s == '.') {
    s++;
    float base = 0.1f;
    while (*s >= '0' && *s <= '9') {
      float digit = (float)(*s++ - '0');
      result += digit * base;
      base *= 0.1f;
    }
  }

  result *= sign;

  // Underflow check and clamp
  if (result != 0.0f && result > -FLT_MIN_ && result < FLT_MIN_) {
    if (error) *error = CALC_STRTOF_UNDERFLOW;
    result = (sign < 0) ? -FLT_MIN_ : FLT_MIN_;
  }

  if (endptr) *endptr = s;
  return result;
}
#else
#include <stdlib.h>
#if CALC_NUM_WIDTH == 32
#define calc_str_to_num strtof
#elif CALC_NUM_WIDTH == 64
#define calc_str_to_num strtod
#endif
#endif

static CalcNum num_data[256];
static CalcCmd cmd_data[256];

CalcBufsResult calc_parse_ascii(const char *str, CalcU8 str_size) {
  CalcU8 num_size = 0;
  CalcU8 cmd_size = 0;
  CalcU8 is_prev_op = 0;

  const char *cur = str;
  const char *end = str + str_size;

  while (cur < end) {
    char c = *cur;
    printf("next char %c\n", c);

    // Ignore whitespace
    if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
      cur++;
      continue;
    }

    // Number parsing
    if (c >= '0' && c <= '9') {
      char *num_end;
      num_data[num_size++].val = calc_str_to_num(cur, &num_end);
      if (num_end >= end) break; // avoid reading past input
      cur = num_end;
      is_prev_op = 0;
      continue;
    }

    // Operator and parenthesis parsing
    switch (c) {
    case '+':
      if (cmd_size == 0 || is_prev_op) {
        cur++;
        continue; // Unary plus, skip
      }
      cmd_data[cmd_size++] = CALC_CMD_ADD;
      is_prev_op = 1;
      cur++;
      continue;

    case '-':
      if (cmd_size == 0) {
        cmd_data[cmd_size++] = CALC_CMD_NEG;
        is_prev_op = 1;
        cur++;
        continue;
      }
      if (!is_prev_op || cmd_data[cmd_size - 1] == CALC_CMD_R_BRACE) {
        cmd_data[cmd_size++] = CALC_CMD_SUB;
        is_prev_op = 1;
        cur++;
        continue;
      }
      cmd_data[cmd_size++] = CALC_CMD_NEG;
      cur++;
      continue;

    case '*':
      cmd_data[cmd_size++] = CALC_CMD_MUL;
      is_prev_op = 1;
      cur++;
      continue;

    case '/':
      cmd_data[cmd_size++] = CALC_CMD_DIV;
      is_prev_op = 1;
      cur++;
      continue;

    case '(':
      cmd_data[cmd_size++] = CALC_CMD_L_BRACE;
      is_prev_op = 1;
      cur++;
      continue;

    case ')':
      cmd_data[cmd_size++] = CALC_CMD_R_BRACE;
      is_prev_op = 1;
      cur++;
      continue;

    default:
      if (c == '.') { return (CalcBufsResult) {CALC_ERR_SYNTAX, {}}; }
      return (CalcBufsResult) {CALC_ERR_UNKNOWN_CHAR, {}};
    }
  }

  return (CalcBufsResult) {CALC_ERR_NONE, {{num_data, num_size}, {cmd_data, cmd_size}}};
}

#endif
