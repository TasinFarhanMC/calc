#include "calc.h"

static CalcCmd stack[CALC_BUF_SIZE];
static CalcCmd out[CALC_BUF_SIZE];

static CalcU8 prec(CalcCmd op) {
  switch (op) {
  case CALC_CMD_ADD:
  case CALC_CMD_SUB:
    return 1;
  case CALC_CMD_MUL:
  case CALC_CMD_DIV:
    return 2;
  case CALC_CMD_NEG:
    return 3;
  default:
    return 0;
  }
}

CalcCmdsResult calc_gen_rpn(const CalcCmds cmds) {
  CalcU16 out_i = 0;
  CalcU16 stack_i = 0;

  for (CalcU16 i = 0; i < cmds.size; ++i) {
    CalcCmd cmd = cmds.data[i];

    switch (cmd) {
    case CALC_CMD_LOAD:
      if (out_i >= CALC_BUF_SIZE) return (CalcCmdsResult) {CALC_ERR_NUM_OVERFLOW};
      out[out_i++] = cmd;
      continue;

    case CALC_CMD_NEG:
    case CALC_CMD_L_BRACE:
      if (stack_i >= CALC_BUF_SIZE) return (CalcCmdsResult) {CALC_ERR_STACK_OVERFLOW};
      stack[stack_i++] = cmd;
      continue;

    case CALC_CMD_R_BRACE:
      while (stack_i > 0 && stack[stack_i - 1] != CALC_CMD_L_BRACE) {
        if (out_i >= CALC_BUF_SIZE) return (CalcCmdsResult) {CALC_ERR_NUM_OVERFLOW};
        out[out_i++] = stack[--stack_i];
      }

      if (stack_i > 0 && stack[stack_i - 1] == CALC_CMD_L_BRACE) --stack_i; // Discard left paren
      continue;

    // Operators
    default:
      while (stack_i > 0) {
        CalcCmd top = stack[stack_i - 1];
        if (top == CALC_CMD_L_BRACE) break;

        if (prec(cmd) < prec(top) || (prec(cmd) == prec(top) && cmd != CALC_CMD_NEG)) {
          if (out_i >= CALC_BUF_SIZE) return (CalcCmdsResult) {CALC_ERR_NUM_OVERFLOW};
          out[out_i++] = stack[--stack_i];
        } else {
          break;
        }
      }

      if (stack_i >= CALC_BUF_SIZE) return (CalcCmdsResult) {CALC_ERR_STACK_OVERFLOW};
      stack[stack_i++] = cmd;
      continue;
    }
  }

  while (stack_i > 0) {
    if (out_i >= CALC_BUF_SIZE) return (CalcCmdsResult) {CALC_ERR_NUM_OVERFLOW};
    out[out_i++] = stack[--stack_i];
  }

  return (CalcCmdsResult) {CALC_ERR_NONE, {out, out_i}};
}
