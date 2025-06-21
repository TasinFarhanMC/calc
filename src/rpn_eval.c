#include "calc.h"

static CalcNum stack[CALC_BUF_SIZE];

CalcNumResult calc_eval_rpn(const CalcBufs bufs) {
  CalcU16 sp = 0;    // Stack pointer
  CalcU16 num_i = 0; // Index into bufs.nums

  for (CalcU16 i = 0; i < bufs.cmds.size; i++) {
    CalcCmd cmd = bufs.cmds.data[i];

    switch (cmd) {
    case CALC_CMD_LOAD:
      if (sp >= CALC_BUF_SIZE) {
        return (CalcNumResult) {CALC_ERR_NUM_OVERFLOW}; // Stack overflow
      }
      if (num_i >= bufs.nums.size) {
        return (CalcNumResult) {CALC_ERR_NUM_OVERFLOW}; // Not enough numbers
      }
      stack[sp++] = bufs.nums.data[num_i++];
      break;

    case CALC_CMD_ADD:
      if (sp < 2) return (CalcNumResult) {CALC_ERR_UNKNOWN_CHAR}; // Not enough operands
      stack[sp - 2].val += stack[sp - 1].val;
      sp--;
      break;

    case CALC_CMD_SUB:
      if (sp < 2) return (CalcNumResult) {CALC_ERR_UNKNOWN_CHAR};
      stack[sp - 2].val -= stack[sp - 1].val;
      sp--;
      break;

    case CALC_CMD_MUL: {
      if (sp < 2) return (CalcNumResult) {CALC_ERR_UNKNOWN_CHAR};
      CalcNumResult res = calc_mul_num(stack[sp - 2], stack[sp - 1]);
      if (res.err != CALC_ERR_NONE) return res;
      stack[sp - 2] = res.ok;
      sp--;
      break;
    }

    case CALC_CMD_DIV: {
      if (sp < 2) return (CalcNumResult) {CALC_ERR_UNKNOWN_CHAR};
      CalcNumResult res = calc_div_num(stack[sp - 2], stack[sp - 1]);
      if (res.err != CALC_ERR_NONE) return res;
      stack[sp - 2] = res.ok;
      sp--;
      break;
    }

    case CALC_CMD_NEG:
      if (sp < 1) return (CalcNumResult) {CALC_ERR_UNKNOWN_CHAR};
      stack[sp - 1].val = -stack[sp - 1].val;
      break;

    default:
      return (CalcNumResult) {CALC_ERR_UNKNOWN_CHAR}; // Invalid operator
    }
  }

  if (sp != 1) {
    return (CalcNumResult) {CALC_ERR_UNKNOWN_CHAR}; // Invalid final stack state
  }

  return (CalcNumResult) {CALC_ERR_NONE, stack[0]};
}
