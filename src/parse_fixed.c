#ifdef CALC_NUM_FIXED
#include "calc.h"

static CalcNum num_data[CALC_BUF_SIZE];
static CalcCmd cmd_data[CALC_BUF_SIZE];

CalcBufsResult calc_parse_ascii(const char *str, CalcU16 str_size) {
  CalcU16 num_size = 0;
  CalcU16 cmd_size = 0;

  CalcU8 is_num = 0;
  CalcU8 is_frac = 0;
  CalcU8 is_prev_op = 0;
  CalcNum num = {0};
  CalcNum frac = {0};
  CalcUSize frac_div = 1;

  for (int i = 0; i < str_size; i++) {
    char c = str[i];

    // Ignore whitespace
    switch (c) {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      continue;
    case '.':
      if (is_frac) {
        return (CalcBufsResult) {CALC_ERR_INVALID_FRAC, {}}; // Already in fractional part — invalid syntax;
      }

      is_frac = 1;
      is_num = 1;
      continue;
    }

    if (c >= '0' && c <= '9') {
      CalcU8 digit = c - '0';
      CalcUHalf max = ((CalcUHalf)-2) / 10; // Max Vlaue / 10 - 10 so that when adding digit it doesnt overflow

      if (is_frac) {
        // If fractional part overflowed before, skip more digits
        if (frac.high > max) { continue; } // ignore digit (overflow in fractional part)

        frac.high = frac.high * 10 + digit;
        frac_div *= 10;
        is_num = 1;
        continue;
      }

      if (num.high > max) { return (CalcBufsResult) {CALC_ERR_NUM_OVERFLOW, {}}; } // throw hard error for integer overflow

      num.high = num.high * 10 + digit;
      is_num = 1;
      continue;
    }

    // If we get here, c is not digit or dot.
    // If a number was being parsed, push it:
    if (is_num) {
      is_prev_op = 0;
      // Calculate final number with fractional part
      // Add 0.5 to frac.val for rounding: (frac.val + frac_div/2) / frac_div
      num.val = num.val + (frac.val + (frac_div >> 1)) / frac_div;
      num_data[num_size++] = num;
      cmd_data[cmd_size++] = CALC_CMD_LOAD;

      // Reset for next number
      num.val = 0;
      frac.val = 0;
      frac_div = 1;
      is_num = 0;
      is_frac = 0;
    }

    switch (c) {
    case '+':
      if (cmd_size == 0 || is_prev_op) { continue; } // Unary Plus

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
      is_prev_op = 1;
      cmd_data[cmd_size++] = CALC_CMD_MUL;
      continue;
    case '/':
      is_prev_op = 1;
      cmd_data[cmd_size++] = CALC_CMD_DIV;
      continue;
    case '(':
      is_prev_op = 1;
      cmd_data[cmd_size++] = CALC_CMD_L_BRACE;
      continue;
    case ')':
      is_prev_op = 1;
      cmd_data[cmd_size++] = CALC_CMD_R_BRACE;
      continue;
    default:
      return (CalcBufsResult) {CALC_ERR_UNKNOWN_CHAR, {}};
    }
  }

  // If string ended but number is still being parsed, push it:
  if (is_num) {
    num.val = num.val + (frac.val + (frac_div >> 1)) / frac_div;
    num_data[num_size++] = num;
    cmd_data[cmd_size++] = CALC_CMD_LOAD;
  }

  return (CalcBufsResult) {CALC_ERR_NONE, {{num_data, num_size}, {cmd_data, cmd_size}}};
}
#endif /* ifdef CALC_NUM_FIXED */
