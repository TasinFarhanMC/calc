#ifdef CALC_NUM_FIXED
#include "calc.h"

static CalcNum num_data[256];
static CalcCmd cmd_data[256];

CalcBufs calc_parse_ascii(const char *str, CalcU8 str_size) {
  CalcU8 num_size = 0;
  CalcU8 cmd_size = 0;

  CalcU8 is_num = 0;
  CalcU8 is_frac = 0;
  CalcNum num = {0};
  CalcNum frac = {0};
  CalcUSize frac_div = 1;

  for (int i = 0; i < str_size; i++) {
    char c = str[i];

    switch (c) {
    case '.':
      is_num = 1;
      is_frac = 1;
      continue;
    case '+':
      cmd_data[cmd_size++] = CALC_CMD_ADD;
      continue;
    case ' ':
    case '\n':
    case '\t':
    case '\r':
      continue;
    }

    if (c >= '0' && c <= '9') {
      if (is_frac) {
        frac.high = frac.high * 10 + c - '0';
        frac_div *= 10;
        continue;
      }

      num.high = num.high * 10 + c - '0';
      is_num = 1;
      continue;
    }

    if (is_num) {
    NUM:
      num_data[num_size++].val = num.val + (frac.val + (frac_div >> 1)) / frac_div;

      num.val = 0;
      frac.val = 0;
      frac_div = 1;
      is_num = 0;
      is_frac = 0;
      cmd_data[cmd_size++] = CALC_CMD_LOAD;
      continue;
    }
  }

  if (is_num) { goto NUM; }

  return (CalcBufs) {{num_data, num_size}, {cmd_data, cmd_size}};
}
#endif /* ifdef CALC_NUM_FIXED */
