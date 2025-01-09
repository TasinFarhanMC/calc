#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define FRACTIONAL_BITS 24

#define OP_FISRT 0x8000000000000000
#define OP_PLUS OP_FISRT
#define OP_B_SUB 0x8000000000000001
#define OP_U_SUB 0x8000000000000002
#define OP_MUL 0x8000000000000003
#define OP_DIV 0x8000000000000004
#define OP_PAREN_R 0x8000000000000005
#define OP_PAREN_L 0x8000000000000006
#define OP_LAST OP_PAREN_R

int64_t process_input(const char *str, int64_t *buffer) {
  int64_t fraction_div = 1;

  int64_t result = 0;
  int64_t fraction = 0;

  uint8_t count = 0;
  bool fractional = false;

  for (; *str; str++) {
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

    const unsigned char c = *str;

    if (c - '0' <= 9) {
      if (fractional) {
        fraction = fraction * 10 + c - '0';
        fraction_div *= 10;
        continue;
      }

      result = result * 10 + c - '0';
      continue;
    }

    switch (c) {
    case '.':
      fractional = true;
      continue;
    case '+':
      buffer[count++] = OP_PLUS;
      continue;
    case '/':
      buffer[count++] = OP_DIV;
      continue;
    case '*':
      buffer[count++] = OP_MUL;
      continue;
    case '(':
      buffer[count++] = OP_PAREN_L;
      continue;
    case ')':
      buffer[count++] = OP_PAREN_R;
      continue;
    case '-':
      if (count > 0 && buffer[count - 1] <= OP_LAST) {
        buffer[count++] = OP_B_SUB;
        continue;
      }

      buffer[count++] = OP_U_SUB;
      continue;
    default:
      continue;
    }
  }

  return count;
}

int main() { process_input((void *)2, (void *)2); }
