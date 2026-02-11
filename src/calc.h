#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CALC_INT
typedef CALC_INT CalcNum;
#define CALC_SHIFT (sizeof(CALC_INT) * 4)

typedef CALC_INT CalcInt;
typedef unsigned CALC_INT CalcUint;

#define CALC_CAST_NUM(x) ((CalcUint)(x) << CALC_SHIFT)

#define CALC_MAX_FRACT_DIGIT (((CALC_SHIFT - 1) * 30103UL / 100000UL) + 1)
#elif defined(CALC_FLOAT)
typedef CALC_FLOAT CalcNum;
typedef CALC_FLOAT CalcFloat;

typedef long long CalcInt;
typedef unsigned long long CalcUint;

#define CALC_CAST_NUM(x) ((CalcNum)(x))
#define CALC_MAX_FRACT_DIGIT (20) // TODO: CREATE MAX FOR FLOATING TYPE
#else
#error "NEITHER CALC_INT NOR CALC_FLOAT DEFINED"
#endif // def CALC_INT

#ifndef CALC_STACK_SIZE
#define CALC_STACK_SIZE 512
#endif // !CALC_STACK_SIZE

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
  CalcUint length;
  CalcUint capacity;
} CalcNumData;

typedef struct {
  CalcCmd *data;
  CalcUint length;
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

#ifdef __cplusplus

#define CALC_NUM_DATA_NULL CalcNumData{0,0,0}
#define CALC_CMD_DATA_NULL CalcCmdData{0,0,0}

#define CALC_NUM_DATA(ptr, capacity) CalcNumData{ (ptr), 0, (capacity) }
#define CALC_CMD_DATA(ptr, capacity) CalcCmdData{ (ptr), 0, (capacity) }

#else

#define CALC_NUM_DATA_NULL (CalcNumData){0,0,0}
#define CALC_CMD_DATA_NULL (CalcCmdData){0,0,0}

#define CALC_NUM_DATA(ptr, capacity) (CalcNumData){ (ptr), 0, (capacity) }
#define CALC_CMD_DATA(ptr, capacity) (CalcCmdData){ (ptr), 0, (capacity) }

#endif

CALC_LINKAGE CalcNum calc_ascii_to_num(const CalcByte *str, const CalcByte **end, const CalcByte *stop); // Nullable end
CALC_LINKAGE CalcError calc_parse_ascii(const CalcByte *str, CalcUint size, CalcNumData *num_data, CalcCmdData *cmd_data);
CALC_LINKAGE CalcError
calc_parse_ascii_till(const CalcByte *str, const CalcByte *stop, CalcByte c, CalcUint count, CalcNumData *num_data, CalcCmdData *cmd_data);

CALC_LINKAGE CalcError calc_convert_rpn(CalcCmdData cmd_data, CalcCmdData *rpn); // Same Data can struct can be used as output

// CALC_LINKAGE CalcNum calc_evaluate_rpn(CalcCmdData cmd_data, CalcNumData num_data, CalcError *error);              // Nullable error
// CALC_LINKAGE CalcNum calc_evaluate_rpn_fast(CalcCmdData cmd_data, CalcNumData num_data, CalcByte *stack_overflow); // Nullable stack_overflow

// Uses num data buffer as the stack result will be in num_data.data[0]
// CALC_LINKAGE CalcError calc_evaluate_rpn_inplace(CalcCmdData cmd_data, CalcNumData num_data);
// Uses num data buffer as the stack
CALC_LINKAGE CalcNum calc_evaluate_rpn_fast_inplace(CalcCmdData cmd_data, CalcNumData num_data);

// TODO:
// CalcUInt calc_fixed_to_asciz(CalcNum, CalcByte*, ...);
// CalcUInt calc_fixed_to_asciz_with_alloc(CalcNum. CalcByte**, ...);

// #ifdef CALC_ALLOC
// CALC_LINKAGE CalcError calc_parse_ascii_with_alloc(const CalcByte *str, CalcUint size, CalcNumData *num_data, CalcCmdData *cmd_data);
// CALC_LINKAGE CalcError calc_parse_ascii_till_with_alloc(const CalcByte *str, CalcByte c, CalcUint count, CalcNumData *num_data, CalcCmdData
// *cmd_data);
//
// CALC_LINKAGE CalcError calc_convert_rpn_with_alloc(CalcCmdData cmd_data, CalcCmdData *rpn); // Same Data can struct can be used as output
// CALC_LINKAGE CalcNum calc_evaluate_rpn_with_alloc(CalcCmdData cmd_data, CalcNumData num_data, CalcError *error); // Nullable error
// CALC_LINKAGE CalcNum calc_evaluate_rpn_fast_with_alloc(CalcCmdData cmd_data, CalcNumData num_data);
// #endif

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

CALC_LINKAGE const CalcByte *calc_debug_str(CalcError error);

#ifdef __cplusplus
}
#endif

#if defined(CALC_IMPLEMENTATION) || defined(CALC_STATIC_IMPLEMENTATION)

CALC_LINKAGE const CalcByte *calc_debug_str(CalcError error) {
  switch (error) {
  case CALC_ERROR_NONE:
    return "No Error";
  case CALC_ERROR_UNKNOWN_SYMBOL:
    return "Unknown Symbol";
  case CALC_ERROR_OVERFLOW:
    return "Overflow Occurred";
  case CALC_ERROR_UNDERFLOW:
    return "Underflow Occurred";
  case CALC_ERROR_DIVISION_ZERO:
    return "Division by Zero";
  case CALC_ERROR_DATA_OVERFLOW:
    return "Data Buffer Overflow";
  default:
    return "Unknown Rrror";
  }
}

CALC_LINKAGE CalcNum calc_ascii_to_num(const CalcByte *str, const CalcByte **end, const CalcByte *stop) {
  CalcNum num = 0;
  CalcInt sign = 1;

  const CalcByte *ptr = str;

  if (*ptr == '-') {
    sign = -1;
    ptr++;
  } else if (*ptr == '+') {
    ptr++;
  }

  while (ptr < stop && *ptr >= '0' && *ptr <= '9') { num = num * 10 + *ptr++ - '0'; }
  num = CALC_CAST_NUM(num);

  if (ptr == stop) { num *= sign; goto END;}

  if (*ptr == '.') {
    ptr++;

    CalcNum fract = 0;
    CalcUint scale = 1;
    CalcUint digits = 0;

    while (ptr < stop && *ptr >= '0' && *ptr <= '9' && ++digits < CALC_MAX_FRACT_DIGIT) {
      fract = (fract * 10) + (*ptr++ - '0');
      scale *= 10;
    }

    if (ptr == stop) {
      num += CALC_DIV_VALUE_JUSTIFIED(CALC_CAST_NUM(fract), scale, 1);
      return num * sign;
    }

    if (*ptr >= '5' && *ptr <= '9') {
      fract++;
      ptr++;
    }

    num += CALC_DIV_VALUE_JUSTIFIED(CALC_CAST_NUM(fract), scale, 1);
    while (ptr < stop && *ptr >= '0' && *ptr <= '9') { ptr++; }

    if (ptr == stop) { return num * sign; }
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
    while (*ptr >= '0' && *ptr <= '9' && ptr < stop) { exponent = (exponent * 10) + (*ptr++ - '0'); }

    CalcNum power = 1;
    CalcNum base = 10;

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

END:
  if (end) { *end = ptr; }
  return num;
}

CALC_LINKAGE CalcError calc_parse_ascii(const CalcByte *str, CalcUint size, CalcNumData *num_data, CalcCmdData *cmd_data) {
  const CalcByte *end = str + size;
  CalcUint cmd_count = 0;
  CalcUint num_count = 0;
  CalcByte is_prev_expr = 0;

  while (end != str) {
    if ((unsigned)(*str - '0') <= 9) {
      if (cmd_count != 0 && cmd_data->data[cmd_count - 1] == CALC_CMD_NEG) {
        cmd_count--;
        str--;
      }

      if (cmd_count == cmd_data->capacity || num_count == num_data->capacity) { return CALC_ERROR_DATA_OVERFLOW; }

      num_data->data[num_count++] = calc_ascii_to_num(str, &str, end);
      cmd_data->data[cmd_count++] = CALC_CMD_LOAD;
      is_prev_expr = 1;
      continue;
    }

    const CalcByte op = *str;

    if ((unsigned)(op - 9) <= 4 || op == ' ') { // is white space
      str++;
      continue;
    }

    switch (op) {
    case '+':
      if (is_prev_expr) {
        if (cmd_count == cmd_data->capacity) { return CALC_ERROR_DATA_OVERFLOW; }
        cmd_data->data[cmd_count++] = CALC_CMD_ADD;
        is_prev_expr = 0;
      }

      str++;
      continue;
    case '-': {
      if (is_prev_expr) {
        if (cmd_count == cmd_data->capacity) { return CALC_ERROR_DATA_OVERFLOW; }
        cmd_data->data[cmd_count++] = CALC_CMD_SUB;
        str++;
        is_prev_expr = 0;
        continue;
      }

      CalcUint neg_count = 1;
      while (*++str == '-') { neg_count++; }
      if (neg_count & 1) {
        if (cmd_count == cmd_data->capacity) { return CALC_ERROR_DATA_OVERFLOW; }
        cmd_data->data[cmd_count++] = CALC_CMD_NEG;
        is_prev_expr = 0;
        continue;
      }

      continue;
    }
    }

    is_prev_expr = 0;
    if (cmd_count == cmd_data->capacity) { return CALC_ERROR_DATA_OVERFLOW; }
    switch (op) {
    case '*':
      cmd_data->data[cmd_count++] = CALC_CMD_MUL;
      str++;
      continue;
    case '/':
      cmd_data->data[cmd_count++] = CALC_CMD_DIV;
      str++;
      continue;
    case '(':
      cmd_data->data[cmd_count++] = CALC_CMD_L_BRACE;
      str++;
      continue;
    case ')':
      cmd_data->data[cmd_count++] = CALC_CMD_R_BRACE;
      is_prev_expr = 1;
      str++;
      continue;
    default:
      return CALC_ERROR_UNKNOWN_SYMBOL;
    }
  }

  num_data->length = num_count;
  cmd_data->length = cmd_count;
  return CALC_ERROR_NONE;
}

CALC_LINKAGE CalcError
calc_parse_ascii_till(const CalcByte *str, const CalcByte *stop, CalcByte c, CalcUint count, CalcNumData *num_data, CalcCmdData *cmd_data) {
  CalcUint cmd_count = 0;
  CalcUint num_count = 0;
  CalcUint c_count = 0;
  CalcByte is_prev_expr = 0;

  while (c_count < count && str < stop) {
    if ((unsigned)(*str - '0') <= 9) {
      if (cmd_count != 0 && cmd_data->data[cmd_count - 1] == CALC_CMD_NEG) {
        cmd_count--;
        str--;
      }

      if (cmd_count == cmd_data->capacity || num_count == num_data->capacity) { return CALC_ERROR_DATA_OVERFLOW; }

      num_data->data[num_count++] = calc_ascii_to_num(str, &str, stop);
      cmd_data->data[cmd_count++] = CALC_CMD_LOAD;
      is_prev_expr = 1;
      continue;
    }

    const CalcByte op = *str;

    if (op == c) {
      c_count++;
      str++;
      continue;
    }

    if ((unsigned)(op - 9) <= 4 || op == ' ') { // is white space
      str++;
      continue;
    }

    switch (op) {
    case '+':
      if (is_prev_expr) {
        if (cmd_count == cmd_data->capacity) { return CALC_ERROR_DATA_OVERFLOW; }
        cmd_data->data[cmd_count++] = CALC_CMD_ADD;
        is_prev_expr = 0;
      }

      str++;
      continue;
    case '-': {
      if (is_prev_expr) {
        if (cmd_count == cmd_data->capacity) { return CALC_ERROR_DATA_OVERFLOW; }
        cmd_data->data[cmd_count++] = CALC_CMD_SUB;
        str++;
        is_prev_expr = 0;
        continue;
      }

      CalcUint neg_count = 1;
      while (str < stop && *++str == '-') { neg_count++; }
      if (neg_count & 1) {
        if (cmd_count == cmd_data->capacity) { return CALC_ERROR_DATA_OVERFLOW; }
        cmd_data->data[cmd_count++] = CALC_CMD_NEG;
        is_prev_expr = 0;
        continue;
      }

      continue;
    }
    }

    is_prev_expr = 0;
    if (cmd_count == cmd_data->capacity) { return CALC_ERROR_DATA_OVERFLOW; }
    switch (op) {
    case '*':
      cmd_data->data[cmd_count++] = CALC_CMD_MUL;
      str++;
      continue;
    case '/':
      cmd_data->data[cmd_count++] = CALC_CMD_DIV;
      str++;
      continue;
    case '(':
      cmd_data->data[cmd_count++] = CALC_CMD_L_BRACE;
      str++;
      continue;
    case ')':
      cmd_data->data[cmd_count++] = CALC_CMD_R_BRACE;
      is_prev_expr = 1;
      str++;
      continue;
    default:
      return CALC_ERROR_UNKNOWN_SYMBOL;
    }
  }

  num_data->length = num_count;
  cmd_data->length = cmd_count;
  return CALC_ERROR_NONE;
}

static CalcByte calc_op_precedence[8] = {1, 1, 1, 3, 2, 2, 0, 0};
static CalcByte calc_op_left[8] = {0, 1, 1, 0, 1, 1, 0, 0};

CALC_LINKAGE CalcError calc_convert_rpn(CalcCmdData cmd_data, CalcCmdData *rpn) {
  CalcCmd stack[CALC_STACK_SIZE];
  CalcUint stack_count = 0;
  CalcUint output_count = 0;

  if (cmd_data.length > rpn->capacity) { return CALC_ERROR_OVERFLOW; }

  for (CalcUint i = 0; i < cmd_data.length; i++) {
    CalcCmd cmd = cmd_data.data[i];

    if ((unsigned)cmd - 1 <= 4) { // if normal cmd
      if (stack_count == CALC_STACK_SIZE) { return CALC_ERROR_OVERFLOW; }

      while (stack_count && calc_op_precedence[stack[stack_count - 1]] >= calc_op_precedence[cmd] && calc_op_left[cmd]) {
        rpn->data[output_count++] = stack[--stack_count];
      }

      stack[stack_count++] = cmd;
      continue;
    }

    switch (cmd) {
    case CALC_CMD_LOAD:
      rpn->data[output_count++] = CALC_CMD_LOAD;
      continue;
    case CALC_CMD_L_BRACE:
      if (stack_count == CALC_STACK_SIZE) { return CALC_ERROR_OVERFLOW; }
      stack[stack_count++] = CALC_CMD_L_BRACE;
      continue;
    case CALC_CMD_R_BRACE:
      while (stack_count && stack[stack_count - 1] != CALC_CMD_L_BRACE) {
        rpn->data[output_count++] = stack[stack_count - 1];
        stack_count--;
      }

      if (!stack_count) { return CALC_ERROR_UNKNOWN_SYMBOL; }
      --stack_count;
      continue;
    default:
      return CALC_ERROR_UNKNOWN_SYMBOL;
    }
  }

  while (stack_count) {
    rpn->data[output_count++] = stack[stack_count - 1];
    stack_count--;
  }

  rpn->length = output_count;
  return CALC_ERROR_NONE;
}

CALC_LINKAGE CalcNum calc_evaluate_rpn_fast_inplace(CalcCmdData cmd_data, CalcNumData num_data) {
  CalcNum *stack = num_data.data;
  CalcUint stack_count = 0;
  CalcUint num_index = 0;

  for (CalcUint i = 0; i < cmd_data.length; i++) {
    switch (cmd_data.data[i]) {
    case CALC_CMD_LOAD:
      stack[stack_count++] = num_data.data[num_index++];
      continue;
    case CALC_CMD_ADD:
      stack[stack_count - 2] = stack[stack_count - 2] + stack[stack_count - 1];
      stack_count--;
      continue;
    case CALC_CMD_SUB:
      stack[stack_count - 2] = stack[stack_count - 2] - stack[stack_count - 1];
      stack_count--;
      continue;
    case CALC_CMD_MUL:
      stack[stack_count - 2] = stack[stack_count - 2] * stack[stack_count - 1];
      stack_count--;
      continue;
    case CALC_CMD_DIV:
      stack[stack_count - 2] = stack[stack_count - 2] / stack[stack_count - 1];
      stack_count--;
      continue;
    case CALC_CMD_NEG:
      stack[stack_count - 1] = -stack[stack_count - 1];
      continue;
    default:
      continue;
    }
  }

  return stack[stack_count - 1];
}

#endif // CALC_IMPLEMENTATION
