#include <stdbool.h>
#include <stdint.h>
#include <token.h>

static const int64_t tokens[256] = {
    ['+'] = OP(ADD), ['*'] = OP(MUL), ['/'] = OP(DIV), ['('] = OP(LEF), [')'] = OP(RIG),
};

int lexer(const char *str, int64_t *buffer) {
  uint64_t fraction_div = 1;

  uint64_t result = 0;
  uint64_t fraction = 0;

  uint8_t count = 0;
  bool fractional = false;
  bool number = false;

  for (; *str; str++) {
    const uint8_t c = *str;

    switch (c) {
    case '.':
      fractional = true;
      continue;
    case ' ':
      continue;
    }

    if (c >= '0' && c <= '9') {
      if (fractional) {
        fraction = fraction * 10 + c - '0';
        fraction_div *= 10;
        continue;
      }

      result = result * 10 + c - '0';
      number = true;
      continue;
    }

    if (number) {
      buffer[count++] = NUM(result) + (NUM(fraction) + (fraction_div >> 1)) / fraction_div;

      result = 0;
      fraction = 0;
      fraction_div = 1;
      fractional = false;
      number = false;
    }

    switch (c) {
    case '-':
      if (count == 0) {
        buffer[count++] = OP(NEG);
        continue;
      }

      if (IS_NUM(buffer[count - 1]) || buffer[count - 1] == OP(RIG)) {
        buffer[count++] = OP(SUB);
        continue;
      }

      buffer[count++] = OP(NEG);
      continue;

    case '+':
      if (count == 0 || IS_OP(buffer[count - 1])) { continue; }

      buffer[count++] = OP(ADD);
      continue;

    default:
      if (tokens[c]) { buffer[count++] = tokens[c]; }
    }

    continue;
  }

  return count;
}
