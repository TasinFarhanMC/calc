#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CALC_INT
typedef CALC_INT CalcNum;
#define CALC_SHIFT (sizeof(CALC_INT) / 2)

typedef CALC_INT CalcInt;
typedef unsigned CALC_INT CalcUint;
#elif defined(CALC_FLOAT)
typedef CALC_FLOAT CalcNum;
typedef int CalcInt;
typedef unsigned CalcUint;
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

#define CALC_NUMS (CalcNums) {0, 0, 0}
#define CALC_CMDS (CalcCmds) {0, 0, 0}

#define CALC_NUMS_FROM_ARR(arr) (CalcNums) {arr, 0, 0}
#define CALC_CMDS_FROM_ARR(arr) (CalcCmds) {arr, 0, 0}

CalcErr calc_parse_ascii(const char *str, CalcUint size, CalcNums *nums, CalcCmds *cmds);
CalcErr calc_parse_ascii_to(const char *str, CalcUint pos, char c, CalcNums *nums, CalcCmds *cmds);

CalcErr calc_to_rpn(CalcCmds cmds);
CalcErr calc_gen_rpn(CalcCmds cmds, CalcCmds *rpn);

CalcNum calc_evaluate_rpn(CalcCmds cmds, CalcNums nums, CalcErr *err); // Nullable Err

CalcNum calc_evaluate_ascii(const char *str, CalcUint size, CalcErr *err);           // Nullable Err
CalcNum calc_evaluate_ascii_to(const char *str, CalcUint pos, char c, CalcErr *err); // Nullable Err

#ifdef CALC_ALLOC
CalcErr calc_parse_ascii_with_alloc(const char *str, CalcUint size, CalcNums *nums, CalcCmds *cmds);
CalcErr calc_parse_ascii_to_with_alloc(const char *str, CalcUint pos, char c, CalcNums *nums, CalcCmds *cmds);

CalcErr calc_to_rpn_with_alloc(CalcCmds cmds);
CalcErr calc_gen_rpn_with_alloc(CalcCmds cmds, CalcCmds *rpn);

CalcNum calc_evaluate_rpn_with_alloc(CalcCmds cmds, CalcNums nums, CalcErr *err); // Nullable Err

CalcNum calc_evaluate_ascii_with_alloc(const char *str, CalcUint size, CalcErr *err);           // Nullable Err
CalcNum calc_evaluate_ascii_to_with_alloc(const char *str, CalcUint pos, char c, CalcErr *err); // Nullable Err
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

#define CALC_NUM(x) ((x) << CALC_SHIFT)

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

static CalcErr str_to_num(const char **str, CalcNum *num_ptr) {
  CalcNum num = 0;
  char sign = 1;

  const char *ptr = *str;

  if (*ptr == '-') {
    sign = -1;
    ptr++;
  } else if (*ptr == '+') {
    ptr++;
  }

#ifndef CALC_IGNORE_UNKNOWN_CHAR
#define handle_unknown_char()                                                                                                                          \
  if (*ptr < '0' || *ptr > '9') { return CALC_ERR_INVALID_SYNTAX; }
#else
#define handle_unknown_char()                                                                                                                          \
  while (*ptr < '0' || *ptr > '9') ptr++
#endif // CALC_IGNORE_UNKNOWN_CHAR

  // Integer part
  while (*ptr >= '0' && *ptr <= '9') { num = num * 10 + CALC_NUM(*ptr++ - '0'); }

  // Fractional part
  if (*ptr == '.') {
    ptr++;

    CalcNum frac = 0;
    CalcUint div = 1;

    handle_unknown_char();

    while (*ptr >= '0' && *ptr <= '9') {
      frac += CALC_NUM(*ptr++ - '0');
      div *= 10;
    }

    num += frac / div;
  }

  // Exponential part
  if (*ptr == 'e' || *ptr == 'E') {
    ptr++;

    char exp_sign = 1;

    if (*ptr == '-') {
      exp_sign = -1;
      ptr++;
    } else if (*ptr == '+') {
      ptr++;
    }

    handle_unknown_char();

    CalcUint exponent = 0;
    while (*ptr >= '0' && *ptr <= '9') { exponent = exponent * 10 + CALC_NUM(*ptr++ - '0'); }

    CalcUint scale = 1;
    for (CalcUint i = 0; i < exponent; ++i) scale *= 10;

    if (exp_sign < 0)
      num /= scale;
    else
      num *= scale;
  }

  // if (num == CALC_FSIZE_INF) { return CALC_ERR_NUM_OVERFLOW; }

  num *= sign;
  *str = ptr; // update caller pointer to the new position
  return CALC_ERR_NONE;
}

#endif // CALC_IMPLEMENTATION
