#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define FRACTIONAL_BITS 24

#define OP_FISRT 0x8000000000000000
#define OP_PLUS OP_FISRT
#define OP_B_SUB 0x8000000000000001
#define OP_U_SUB 0x8000000000000002
#define OP_MUL 0x8000000000000003
#define OP_DIV 0x8000000000000004
#define OP_PAREN_L 0x8000000000000005
#define OP_PAREN_R 0x8000000000000006
#define OP_LAST OP_PAREN_R

int64_t string_to_fixed_array(const char *str, int64_t *buffer) {
  int64_t count = 0;
  int64_t result = 0;
  int64_t fractional_part = 0;
  bool fractional = false;
  int fractional_div = 1;

  while (*str == ' ') {
    str++;
  }

  for (; *str; str++) {
    switch (*str) {
    case ' ':
      continue;
    case '.':
      fractional = true;
      continue;
    case '(':
      buffer[count++] = OP_PAREN_L;
      continue;
    }

    if (*str >= '0' && *str <= '9') {
      if (!fractional) {
        result = result * 10 + *str - '0';
        continue;
      }

      fractional_part = fractional_part * 10 + *str - '0';
      fractional_div *= 10;
      continue;
    }

    if (!count && *str == '-') {
      buffer[count++] = OP_U_SUB;
      continue;
    }

    buffer[count++] =
        (result << FRACTIONAL_BITS) +
        ((fractional_part << FRACTIONAL_BITS) + (fractional_div >> 1)) /
            fractional_div;
    ;

    result = 0;
    fractional_part = 0;
    fractional = false;
    fractional_div = 1;
    goto switch_char;

  next_charecter:
    str++;
    while (*str == ' ') {
      str++;
    }

  switch_char:
    switch (*str) {
    case '+':
      buffer[count++] = OP_PLUS;
      continue;
    case '-':
      if (count > 0 && (buffer[count - 1] > (int64_t)OP_LAST ||
                        buffer[count - 1] == OP_PAREN_L)) {
        printf("Previous item: %ld\n", buffer[count - 1]);
        buffer[count++] = OP_B_SUB;
        continue;
      }

      buffer[count++] = OP_U_SUB;
      goto next_charecter;
    case '/':
      buffer[count++] = OP_DIV;
      continue;
    case '*':
      buffer[count++] = OP_MUL;
      continue;
    case ')':
      buffer[count++] = OP_PAREN_R;
      goto next_charecter;
    }
  }

  return count;
}

#define BUFFER_SIZE 256

const int64_t precendance[] = {1, 1, 1, 2, 2, 0, 0};

int64_t shunting_yard(const int64_t *input, const int64_t size, int64_t *out) {
  int64_t stack[BUFFER_SIZE];
  int64_t top = 0;
  int64_t count = 0;

  for (int i = 0; i < size; i++) {
    const int64_t item = input[i];

    if (item > (int64_t)OP_LAST) {
      out[count++] = item;
      continue;
    }

    if (!top) {
      stack[top++] = item;
      continue;
    }

    if (item == OP_PAREN_R) {
      while (stack[--top] != OP_PAREN_L) {
        out[count++] = stack[top];
      }
      continue;
    }

    while (top &&
           precendance[item - OP_FISRT] <=
               precendance[(uint64_t)stack[top - 1] - (uint64_t)OP_FISRT]) {
      out[count++] = stack[--top];
    }

    stack[top++] = item;
  }

  while (top) {
    out[count++] = stack[--top];
  }

  return count;
}

int main() {
  char input[BUFFER_SIZE];
  fgets(input, BUFFER_SIZE, stdin);

  int64_t buf[BUFFER_SIZE];
  size_t size = string_to_fixed_array(input, buf);

  int64_t token[BUFFER_SIZE];
  size_t p_size = shunting_yard(buf, size, token);

  printf("OP plus %ld\n", OP_PLUS);
  printf("OP b sub %ld\n", OP_B_SUB);
  printf("OP u sub %ld\n", OP_U_SUB);
  printf("OP mul %ld\n", OP_MUL);
  printf("OP div %ld\n", OP_DIV);
  printf("OP ( %ld\n", OP_PAREN_L);
  printf("OP ) %ld\n", OP_PAREN_R);
  puts("");

  for (int j = 0; j < size; j++) {
    printf("Fixed-point representation [%d]: %" PRId64 "\n", j, buf[j]);
  }

  return 0;
}
