#include <stdio.h>
#include <token.h>

int64_t rpn(const int64_t *buffer, int size) {
  int64_t stack[MAX_TOKENS];
  int stack_size = 0;

  for (int i = 0; i < size; i++) {
    if (buffer[i] == OP(NEG)) {
      stack[stack_size - 1] = -stack[stack_size - 1];
      continue;
    }

    if (buffer[i] > OP(DIV)) {
      stack[stack_size++] = buffer[i];
      continue;
    }

    int64_t op = buffer[i];
    uint64_t b = stack[--stack_size];
    uint64_t a = stack[--stack_size];
    int64_t res;

    switch (op) {
    case OP(ADD):
      res = a + b;
      break;
    case OP(SUB):
      res = a - b;
      break;
    case OP(MUL):
      __int128_t ra = (__int128_t)a * (__int128_t)b;
      res = (int64_t)(ra >> FRACTIONAL_BITS);
      break;
    case OP(DIV):
      __int128_t rb = (__int128_t)a << 32;
      res = (int64_t)(rb / b);
      break;
    default:
      res = 0;
      break;
    }

    stack[stack_size++] = res;
  }

  return stack[0];
}
