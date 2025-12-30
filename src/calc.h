#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CALC_INT
typedef CALC_INT CalcNum;
#define CALC_SHIFT (sizeof(CALC_INT) * 8 / 2)

typedef CALC_INT CalcInt;
typedef unsigned CALC_INT CalcUint;

#define CALC_CAST_NUM(x) (CalcNum(x) << CALC_SHIFT)
#elif defined(CALC_FLOAT)
typedef CALC_FLOAT CalcNum;
typedef CALC_FLOAT CalcFloat;

typedef long CalcInt;
typedef unsigned long CalcUint;

#define CALC_CAST_NUM(x) (CalcNum(x))
#else
#error "NEITHER CALC_INT NOR CALC_FLOAT DEFINED"
#endif // def CALC_INT

typedef char CalcByte;

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
  CalcUint filled_size;
  CalcUint capacity;
} CalcNumData;

typedef struct {
  CalcCmd *data;
  CalcUint filled_size;
  CalcUint capacity;
} CalcCmdData;

typedef enum {
  CALC_ERROR_NONE,
  CALC_ERROR_UNKNOWN_SYMBOL,
  CALC_ERROR_OVERFLOW,
  CALC_ERROR_UNDERFLOW,
  CALC_ERROR_DIVISION_ZERO,
  CALC_ERROR_DATA_OVERFLOW,
} CalcError;

#ifdef CALC_STATIC_IMPLEMENTATION
#define CALC_LINKAGE static
#else
#define CALC_LINKAGE extern
#endif

#define CALC_NUM_DATA_NULL (CalcNumData) {0, 0, 0}
#define CALC_CMD_DATA_NULL (CalcCmdData) {0, 0, 0}

#define CALC_NUM_DATA(ptr, capacity)                                                                                                                   \
  (CalcNumData) { (ptr), 0, (capacity) }
#define CALC_CMD_DATA(ptr, capacity)                                                                                                                   \
  (CalcCmdData) { (ptr), 0, (capacity) }

CALC_LINKAGE CalcError calc_parse_ascii(const CalcByte *str, CalcUint size, CalcNumData *nums, CalcCmdData *cmds);
CALC_LINKAGE CalcError calc_parse_ascii_to(const CalcByte *str, CalcUint pos, CalcNumData *nums, CalcCmdData *cmds);
CALC_LINKAGE CalcError calc_parse_ascii_till(const CalcByte *str, CalcByte c, CalcUint count, CalcNumData *nums, CalcCmdData *cmds);

CALC_LINKAGE CalcError calc_to_rpn(CalcCmdData cmds);
CALC_LINKAGE CalcError calc_gen_rpn(CalcCmdData cmds, CalcCmdData *rpn);

CALC_LINKAGE CalcNum calc_evaluate_rpn(CalcCmdData cmds, CalcNumData nums, CalcError *error); // Nullable error

CALC_LINKAGE CalcNum calc_evaluate_ascii(const CalcByte *str, CalcUint size, CalcError *error);               // Nullable error
CALC_LINKAGE CalcNum calc_evaluate_ascii_to(const CalcByte *str, CalcUint pos, CalcByte c, CalcError *error); // Nullable error

CALC_LINKAGE CalcNum calc_ascii_to_num(const CalcByte *str, const CalcByte **end, CalcError *error); // Nullable end, error

// TODO:
// CalcUInt calc_fixed_to_asciz(CalcNum, CalcByte*, ...);
// CalcUInt calc_fixed_to_asciz_with_alloc(CalcNum. CalcByte**, ...);

#ifdef CALC_ALLOC
CALC_LINKAGE CalcError calc_parse_ascii_with_alloc(const CalcByte *str, CalcUint size, CalcNumData *nums, CalcCmdData *cmds);
CALC_LINKAGE CalcError calc_parse_ascii_to_with_alloc(const CalcByte *str, CalcUint pos, CalcNumData *nums, CalcCmdData *cmds);
CALC_LINKAGE CalcError calc_parse_ascii_till_with_alloc(const CalcByte *str, CalcByte c, CalcUint count, CalcNumData *nums, CalcCmdData *cmds);

CALC_LINKAGE CalcError calc_to_rpn_with_alloc(CalcCmdData cmds);
CALC_LINKAGE CalcError calc_gen_rpn_with_alloc(CalcCmdData cmds, CalcCmdData *rpn);

CALC_LINKAGE CalcNum calc_evaluate_rpn_with_alloc(CalcCmdData cmds, CalcNumData nums, CalcError *error); // Nullable error

CALC_LINKAGE CalcNum calc_evaluate_ascii_with_alloc(const CalcByte *str, CalcUint size, CalcError *error);               // Nullable error
CALC_LINKAGE CalcNum calc_evaluate_ascii_to_with_alloc(const CalcByte *str, CalcUint pos, CalcByte c, CalcError *error); // Nullable error
#endif

#ifdef CALC_INT
#define CALC_DIV_VALUE_JUSTIFIED(a, x, sign) (((a) + (sign) * ((x) / 2)) / (x))

CALC_LINKAGE CalcNum calc_mul_fixed_safe(CalcNum a, CalcNum b, CalcError *error);
CALC_LINKAGE CalcNum calc_div_fixed_safe(CalcNum a, CalcNum b, CalcError *error);

#ifdef CALC_WIDE_INT
#define calc_mul_fixed()
#define calc_div_fixed()
#else
CALC_LINKAGE CalcNum calc_mul_fixed(CalcNum a, CalcNum b);
CALC_LINKAGE CalcNum calc_div_fixed(CalcNum a, CalcNum b);
#endif // CALC_WIDE_INT
#else
#define CALC_DIV_VALUE_JUSTIFIED(a, x, sign) (a / x)
#endif // CALC_INT

const CalcByte *calc_debug_str(CalcError error);

#ifdef __cplusplus
}
#endif

#if defined(CALC_IMPLEMENTATION) || defined(CALC_STATIC_IMPLEMENTATION)

CALC_LINKAGE CalcNum calc_ascii_to_num(const CalcByte *str, const CalcByte **end, CalcError *error) {
  CalcNum num = 0;
  CalcInt sign = 1;

  const CalcByte *ptr = str;

  if (*ptr == '-') {
    sign = -1;
    ptr++;
  } else if (*ptr == '+') {
    ptr++;
  }

  while (*ptr >= '0' && *ptr <= '9') { num = num * 10 + CALC_CAST_NUM(*ptr++ - '0'); }

  if (*ptr == '.') {
    ptr++;

    CalcUint fract = 0;
    CalcUint scale = 1;

    while (*ptr >= '0' && *ptr <= '9') {
      fract = (fract * 10) + (*ptr++ - '0');
      scale *= 10;
    }

    num += CALC_DIV_VALUE_JUSTIFIED(CALC_CAST_NUM(fract), scale, 1);
  }

  num *= sign;

  if (*ptr == 'e' || *ptr == 'E') {
    ptr++;
    int exp_neg = 0;

    if (*ptr == '-') {
      exp_neg = 1;
      ptr++;
    } else if (*ptr == '+') {
      ptr++;
    }

    CalcUint exponent = 0;
    while (*ptr >= '0' && *ptr <= '9') { exponent = (exponent * 10) + (*ptr++ - '0'); }

    CalcInt power = 1;
    CalcUint base = 10;

    while (exponent > 0) {
      if (exponent & 1) { power *= base; }
      base *= base;
      exponent >>= 1;
    }

    if (exp_neg) {
      num = CALC_DIV_VALUE_JUSTIFIED(num, power, sign);
    } else {
      num *= power;
    }
  }

  if (error) { *error = CALC_ERROR_NONE; }
  if (end) { *end = ptr; }

  return num;
}

#endif // CALC_IMPLEMENTATION
