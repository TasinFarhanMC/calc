#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define FRACTIONAL_BITS 32

#define OP_FIRST 0x8000000000000000
#define OP_PAREN_L (OP_FIRST + 0)
#define OP_PAREN_R (OP_FIRST + 1)
#define OP_MUL (OP_FIRST + 2)
#define OP_PLUS (OP_FIRST + 3)
#define OP_DIV (OP_FIRST + 7)
#define OP_LAST OP_DIV

#define OP_SUB_U (OP_FIRST + 4)
#define OP_SUB_B (OP_FIRST + 5)

int64_t process_input(const char *str, uint64_t *buffer) {
  uint64_t fraction_div = 1;
  uint64_t a = OP_PAREN_R;

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
      if ((count > 0 && buffer[count - 1] <= OP_PAREN_R)) {
        buffer[count++] = OP_SUB_B;
        continue;
      }

      buffer[count++] = OP_SUB_U;
      continue;
    }

    if ((uint8_t)(c - '(') <= '/') {
      buffer[count++] = c - '(' + OP_FIRST;
      continue;
    }
  }

  return count;
}

char str[1024];
uint64_t buf[1024];

int main(void) {
  fgets(str, 1024, stdin);
  int64_t count = process_input(str, buf);

  printf("+: %lu\n", OP_PLUS);
  printf("-: %lu\n", OP_SUB_B);
  printf("*: %lu\n", OP_MUL);
  printf("/: %lu\n", OP_DIV);
  printf("(-): %lu\n", OP_SUB_U);
  printf("(: %lu\n", OP_PAREN_L);
  printf("): %lu\n", OP_PAREN_R);
  puts("");

  for (int i = 0; i < count; i++) {
    printf("Token: %lu\n", buf[i]);
  }
  return 0;
}
