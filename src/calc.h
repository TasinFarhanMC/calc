#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CALC_INT
typedef CALC_INT CalcNum;
#define CALC_SHIFT (sizeof(CALC_INT) << 2)

typedef CALC_INT CalcInt;
typedef unsigned CALC_INT CalcUInt;
#elif defined(CALC_FLOAT)
typedef CALC_FLOAT CalcNum;
typedef int CalcInt;
typedef unsigned CalcUInt;
#else
#error "NEITHER CALC_INT NOR CALC_FLOAT DEFINED"
#endif // def CALC_INT

typedef enum {
  CALC_CMD_LOAD,
  CALC_CMD_ADD,
  CALC_CMD_SUB,
  CALC_CMD_NEG,
  CALC_CMD_MUL,
  CALC_CMD_DIV,
  CALC_CMD_L_BRACE,
  CALC_CMD_R_BRACE,
} CalcCmd;

typedef struct {
  CalcNum *data;
  CalcInt size;
  CalcInt cap;
} CalcNums;

typedef struct {
  CalcCmd *data;
  CalcInt size;
  CalcInt cap;
} CalcCmds;

typedef enum {
  CALC_ERR_NONE,
  CALC_ERR_UNKNOWN_CHAR,
  CALC_ERR_NUM_OVERFLOW,
  CALC_ERR_DIV_ZERO,
  CALC_ERR_STACK_OVERFLOW,
  CALC_ERR_INVALID_SYNTAX,
} CalcErr;

#define CALC_NUMS_FROM_ARR(arr) (CalcNums) {arr, 0, 0}
#define CALC_CMDS_FROM_ARR(arr) (CalcCmds) {arr, 0, 0}

#define CALC_EMPTY_NUMS (CalcNums) {0, 0, 0}
#define CALC_EMPTY_CMDS (CalcCmds) {0, 0, 0}

CalcErr calc_parse_ascii(const char *str, CalcUInt size, CalcNums *nums, CalcCmds *cmds);
CalcErr calc_parse_ascii_to(const char *str, CalcUInt pos, char c, CalcNums *nums, CalcCmds *cmds);

CalcErr calc_to_rpn(CalcCmds cmds);
CalcErr calc_gen_rpn(CalcCmds cmds, CalcCmds *rpn);

CalcNum calc_evaluate_rpn(CalcCmds cmds, CalcNums nums, CalcErr *err); // Nullable Err

CalcNum calc_evaluate_ascii(const char *str, CalcUInt size, CalcErr *err);           // Nullable Err
CalcNum calc_evaluate_ascii_to(const char *str, CalcUInt pos, char c, CalcErr *err); // Nullable Err

#ifdef CALC_ALLOC
CalcErr calc_parse_ascii_with_alloc(const char *str, CalcUInt size, CalcNums *nums, CalcCmds *cmds);
CalcErr calc_parse_ascii_to_with_alloc(const char *str, CalcUInt pos, char c, CalcNums *nums, CalcCmds *cmds);

CalcErr calc_to_rpn_with_alloc(CalcCmds cmds);
CalcErr calc_gen_rpn_with_alloc(CalcCmds cmds, CalcCmds *rpn);

CalcNum calc_evaluate_rpn_with_alloc(CalcCmds cmds, CalcNums nums, CalcErr *err); // Nullable Err

CalcNum calc_evaluate_ascii_with_alloc(const char *str, CalcUInt size, CalcErr *err);           // Nullable Err
CalcNum calc_evaluate_ascii_to_with_alloc(const char *str, CalcUInt pos, char c, CalcErr *err); // Nullable Err
#endif

#ifdef CALC_INT
#ifdef CALC_WIDE_INT
#define calc_mul_fixed()
#define calc_div_fixed()
#else
CalcNum calc_mul_fixed(CalcNum a, CalcNum b);
CalcNum calc_div_fixed(CalcNum a, CalcNum b);
#endif // CALC_WIDE_INT

CalcNum calc_mul_fixed_safe(CalcNum a, CalcNum b, CalcErr *err);
CalcNum calc_div_fixed_safe(CalcNum a, CalcNum b, CalcErr *err);

#define CALC_NUM(x) (x << CALC_SHIFT)

// TODO:
// CalcUInt calc_fixed_to_asciz(CalcNum, char*, ...);
// CalcUInt calc_fixed_to_asciz_with_alloc(CalcNum. char**, ...);
#elif defined(CALC_FLOAT)
#define CALC_NUM(x) (x)
#endif // CALC_INT

const char *calc_debug_str(CalcErr err);

#ifdef __cplusplus
}
#endif

#ifdef CALC_IMPLEMENTATION

CalcErr calc_parse_ascii(const char *str, CalcUInt size, CalcNums *nums, CalcCmds *cmds) {
  CalcNum num = 0;
  char sign = 1;

  const char *p = str;
  if (*p == '-') {
    sign = -1;
    p++;
  } else if (*p == '+') {
    p++;
  }

#ifndef CALC_IGNORE_UNKNOWN_CHAR
#define check_for_unknown_char()                                                                                                                       \
  if (*p < '0' || *p > '9') return CALC_ERR_INVALID_SYNTAX
#else
#define check_for_unknown_char()                                                                                                                       \
  while (*p < '0' || *p > '9') p++
#endif // CALC_IGNORE_UNKNOWN_CHAR

  // First digit must be numeric
  check_for_unknown_char();
  num += *p++ - '0';

  // Integer part
  while (*p >= '0' && *p <= '9') { num = num * 10 + CALC_NUM(*p++ - '0'); }

  // Fractional part
  if (*p == '.') {
    p++;
    CalcNum frac = 0.0f, base = 0.1f;
    check_for_unknown_char();

    while (*p >= '0' && *p <= '9') {
      frac += (*p++ - '0') * base;
      base *= 0.1f;
    }
    num += frac;
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

    check_for_unknown_char();

    CalcUMax exponent = 0;
    while (*p >= '0' && *p <= '9') { exponent = exponent * 10 + (*p++ - '0'); }

    CalcFSize scale = 1.0f;
    for (CalcUMax i = 0; i < exponent; ++i) scale *= 10;

    if (exp_sign < 0)
      num /= scale;
    else
      num *= scale;
  }

  if (num == CALC_FSIZE_INF) { return CALC_ERR_NUM_OVERFLOW; }

  num *= sign;
  str = p; // update caller pointer to the new position
  return CALC_ERR_NONE;
}
#else
#include <stdlib.h>

CalcError str_to_float(const char *str, CalcFSize num) {
  const char *p = str;
  char *end;
  num = strtold(p, &end);

  if (end == p) { return CALC_ERR_INV_FRAC; }
  if (num == CALC_FSIZE_INF) { return CALC_ERR_NUM_OVERFLOW; }

  str = end;
  return CALC_ERR_NONE;
}
#endif

static CalcNum num_data[256];
static CalcCmd cmd_data[256];

CalcBufsResult calc_parse_ascii(const char *str, CalcU16 str_size) {
  CalcU16 num_size = 0;
  CalcU16 cmd_size = 0;
  CalcU16 is_prev_op = 0;

  const char *end = str + str_size;

  for (const char *cur = str; cur < end; cur++) {
    char c = *cur;

    // Ignore whitespace
    if (c == ' ' || c == '\t' || c == '\r' || c == '\n') { continue; }

    // Number parsing
    if (c >= '0' && c <= '9') {
      CalcError err = str_to_float(&cur, &num_data[num_size++].val);
      if (err) { return (CalcBufsResult) {err}; }
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
      if (c == '.') { return (CalcBufsResult) {CALC_ERR_INV_FRAC}; }
      return (CalcBufsResult) {CALC_ERR_UNKNOWN_CHAR};
    }
  }

  return (CalcBufsResult) {CALC_ERR_NONE, {{num_data, num_size}, {cmd_data, cmd_size}}};
}
}

#endif // CALC_IMPLEMENTATION
