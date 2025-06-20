
#ifdef CALC_NUM_FIXED
#include "calc.h"

static CalcNum num_data[256];
static CalcCmd cmd_data[256];

CalcBufsResult calc_parse_ascii(const char *str, CalcU8 str_size) {
  CalcU8 num_size = 0;
  CalcU8 cmd_size = 0;

  CalcU8 is_num = 0;
  CalcU8 is_frac = 0;
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
        return (CalcBufsResult) {CALC_ERR_SYNTAX, {}}; // Already in fractional part — invalid syntax;
      }

      is_frac = 1;
      is_num = 1;
      continue;
    }

    if (c >= '0' && c <= '9') {
      CalcU8 digit = c - '0';
#define NUM_PART_MAX (((CalcUHalf) - 2) / 10)

      if (is_frac) {
        // If fractional part overflowed before, skip more digits
        if (frac.high > NUM_PART_MAX) { continue; } // ignore digit (overflow in fractional part)

        frac.high = frac.high * 10 + digit;
        frac_div *= 10;
        is_num = 1;
        continue;
      }

      if (num.high > NUM_PART_MAX) { return (CalcBufsResult) {CALC_ERR_NUM_OVERFLOW, {}}; } // throw hard error for integer overflow

      num.high = num.high * 10 + digit;
      is_num = 1;
      continue;
    }

    // If we get here, c is not digit or dot.
    // If a number was being parsed, push it:
    if (is_num) {
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
      cmd_data[cmd_size++] = CALC_CMD_ADD;
      continue;
    case '-':
      cmd_data[cmd_size++] = CALC_CMD_SUB;
      continue;
    default:
      return (CalcBufsResult) {CALC_ERR_UNKNOWN_CHAR, {}};
    }

    // return (CalcBufsResult) {CALC_ERR_SYNTAX, {}}; // Operator Before Number
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
