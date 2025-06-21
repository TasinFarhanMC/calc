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
  CalcU16 stack_i = 0;
  CalcU16 out_i = 0;

  for (int i = 0; i < cmds.size; ++i) {
    const CalcCmd cmd = cmds.data[i];

    if (cmd == CALC_CMD_LOAD) {
      out[out_i++] = cmd;
      continue;
    }

    if (cmd == CALC_CMD_NEG) {
      stack[stack_i++] = cmd;
      continue;
    }

    if (cmd == CALC_CMD_L_BRACE) {
      stack[stack_i++] = cmd;
      continue;
    }

    if (cmd == CALC_CMD_R_BRACE) {
      while (stack_i > 0 && stack[stack_i - 1] != CALC_CMD_L_BRACE) { out[out_i++] = stack[--stack_i]; }
      if (stack_i > 0 && stack[stack_i - 1] == CALC_CMD_L_BRACE) {
        --stack_i; // Discard the left parenthesis.
      }
      continue;
    }

    while (stack_i > 0) {
      CalcCmd top = stack[stack_i - 1];
      if (top == CALC_CMD_L_BRACE) break;

      if (prec(cmd) < prec(top) || (prec(cmd) == prec(top) && cmd != CALC_CMD_NEG)) {
        out[out_i++] = stack[--stack_i];
      } else {
        break;
      }
    }

    stack[stack_i++] = cmd;
  }

  // ✅ Final step: pop remaining ops from stack
  while (stack_i > 0) { out[out_i++] = stack[--stack_i]; }

  return (CalcCmdsResult) {CALC_ERR_NONE, {out, out_i}};
}
