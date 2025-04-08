#include <stdbool.h>
#include <stdint.h>
#include <token.h>

static const uint8_t tokens[256] = {
    ['+'] = ADD, ['*'] = MUL, ['/'] = DIV, ['('] = LEF, [')'] = RIG, ['t'] = T,
};

int lexer(const char *str, int64_t *buffer) {
  uint64_t fraction_div = 1;

  uint64_t result = 0;
  uint64_t fraction = 0;

  int8_t count = 0;
  bool fractional = false;
  bool number = false;

  for (; *str; str++) {
    const char c = *str;

    switch (c) {
    case '.':
      fractional = true;
      number = true;
      continue;
    case ' ':
      continue;
    }

    if (c >= '0' && c <= '9') {
      const uint8_t digit = c - '0';

      if (fractional) {
        fraction = fraction * 10 + digit;
        fraction_div *= 10;
        continue;
      }

      result = result * 10 + digit;
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

      if (!IS_OP(buffer[count - 1]) || buffer[count - 1] == OP(RIG)) {
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
      if (tokens[c]) { buffer[count++] = OP(tokens[c]); }
    }

    continue;
  }

  return count;
}
