#include <stdbool.h>
#include <token.h>

int lexer(const char *str, int64_t *buffer) {
  uint64_t fraction_div = 1;

  uint64_t result = 0;
  uint64_t fraction = 0;

  uint8_t count = 0;
  bool fractional = false;

  for (; *str; str++) {
    const uint8_t c = *str;
    switch (c) {
    case '.':
      fractional = true;
      continue;
    case ' ':
      continue;
    }

    if ((uint8_t)(c - '0') <= 9) {
      if (fractional) {
        fraction = fraction * 10 + c - '0';
        fraction_div *= 10;
        continue;
      }

      result = result * 10 + c - '0';
      continue;
    }

    if (result | fraction) {
      buffer[count++] =
          (result << FRACTIONAL_BITS) +
          ((fraction << FRACTIONAL_BITS) + (fraction_div >> 1)) / fraction_div;
      ;

      result = 0;
      fraction = 0;
      fraction_div = 1;
      fractional = false;
    }

    if (c == '-') {
      if ((count > 0 && buffer[count - 1] <= (uint64_t)OP(PAREN_R))) {
        buffer[count++] = OP(SUB_B);
        continue;
      }

      buffer[count++] = OP(SUB_U);
      continue;
    }

    if ((uint8_t)(c - '(') <= '/') {
      buffer[count++] = c - '(' + OP_FIRST;
      continue;
    }
  }

  return count;
}
