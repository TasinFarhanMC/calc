#ifdef CALC_NUM_FLOAT

#include "calc.h"

#ifndef CALC_USE_STDLIB

CalcError str_to_float(const char **s, CalcFSize *n) {
  *n = 0.0f;
  char sign = 1;
  const char *p = *s;

  if (*p == '-') {
    sign = -1;
    p++;
  } else if (*p == '+') {
    p++;
  }

  // First digit must be numeric
  if (*p < '0' || *p > '9') return CALC_ERR_INVALID_FRAC;
  *n += *p++ - '0';

  // Integer part
  while (*p >= '0' && *p <= '9') { *n = *n * 10 + (*p++ - '0'); }

  // Fractional part
  if (*p == '.') {
    p++;
    CalcFSize frac = 0.0f, base = 0.1f;
    if (*p < '0' || *p > '9') return CALC_ERR_INVALID_FRAC;

    while (*p >= '0' && *p <= '9') {
      frac += (*p++ - '0') * base;
      base *= 0.1f;
    }
    *n += frac;
  }

  // Exponential part
  if (*p == 'e' || *p == 'E') {
    p++;
    char exp_sign = 1;
    if (*p == '-') {
      exp_sign = -1;
      p++;
    } else if (*p == '+') {
      p++;
    }

    if (*p < '0' || *p > '9') return CALC_ERR_INVALID_FRAC;

    CalcU32 exponent = 0;
    while (*p >= '0' && *p <= '9') { exponent = exponent * 10 + (*p++ - '0'); }

    CalcFSize scale = 1.0f;
    for (CalcU32 i = 0; i < exponent; ++i) scale *= 10;

    if (exp_sign < 0)
      *n /= scale;
    else
      *n *= scale;
  }

  if (*n > CALC_FSIZE_MAX) { return CALC_ERR_NUM_OVERFLOW; }

  *n *= sign;
  *s = p; // update caller pointer to the new position
  return CALC_ERR_NONE;
}
#else
#include <stdlib.h>

CalcError str_to_float(const char **s, CalcFSize *n) {
  const char *p = *s;
  char *end;
  *n = strtold(p, &end);

  if (end == p) { return CALC_ERR_INVALID_FRAC; }
  if (*n > CALC_FSIZE_MAX) { return CALC_ERR_NUM_OVERFLOW; }

  *s = end;
  return CALC_ERR_NONE;
}
#endif

static CalcNum num_data[256];
static CalcCmd cmd_data[256];

CalcBufsResult calc_parse_ascii(const char *str, CalcU8 str_size) {
  CalcU8 num_size = 0;
  CalcU8 cmd_size = 0;
  CalcU8 is_prev_op = 0;

  const char *end = str + str_size;

  for (const char *cur = str; cur < end; cur++) {
    char c = *cur;

    // Ignore whitespace
    if (c == ' ' || c == '\t' || c == '\r' || c == '\n') { continue; }

    // Number parsing
    if (c >= '0' && c <= '9') {
      CalcError err = str_to_float(&cur, &num_data[num_size++].val);
      if (err) { return (CalcBufsResult) {err, {}}; }
      cmd_data[cmd_size++] = CALC_CMD_LOAD;

      // Break if reached end
      if (cur == end) { break; }
      is_prev_op = 0;
      c = *cur;
    }

    // Operator and parenthesis parsing
    switch (c) {
    case '+':
      // Unary plus, skip
      if (num_size == 0 || is_prev_op) { continue; }

      cmd_data[cmd_size++] = CALC_CMD_ADD;
      is_prev_op = 1;
      continue;

    case '-':
      if (cmd_size == 0) {
        cmd_data[cmd_size++] = CALC_CMD_NEG;
        is_prev_op = 1;
        continue;
      }

      if (!is_prev_op || cmd_data[cmd_size - 1] == CALC_CMD_R_BRACE) {
        cmd_data[cmd_size++] = CALC_CMD_SUB;
        is_prev_op = 1;
        continue;
      }
      cmd_data[cmd_size++] = CALC_CMD_NEG;
      continue;

    case '*':
      cmd_data[cmd_size++] = CALC_CMD_MUL;
      is_prev_op = 1;
      continue;

    case '/':
      cmd_data[cmd_size++] = CALC_CMD_DIV;
      is_prev_op = 1;
      continue;

    case '(':
      cmd_data[cmd_size++] = CALC_CMD_L_BRACE;
      is_prev_op = 1;
      continue;

    case ')':
      cmd_data[cmd_size++] = CALC_CMD_R_BRACE;
      is_prev_op = 1;
      continue;

    default:
      if (c == '.') { return (CalcBufsResult) {CALC_ERR_INVALID_FRAC, {}}; }
      return (CalcBufsResult) {CALC_ERR_UNKNOWN_CHAR, {}};
    }
  }

  return (CalcBufsResult) {CALC_ERR_NONE, {{num_data, num_size}, {cmd_data, cmd_size}}};
}

#endif
