#include <stdint.h>
#include <token.h> // your token definitions and MAX_TOKENS

// Precedence: higher value means higher precedence.
int precedence(int64_t op) {
  switch (op) {
  case OP(ADD):
  case OP(SUB_B):
    return 1;
  case OP(MUL):
  case OP(DIV):
    return 2;
  case OP(SUB_U):
    return 3; // unary minus gets highest precedence
  default:
    return 0;
  }
}

// Only unary minus (SUB_U) is right-associative.
int is_right_associative(int64_t op) { return op == OP(SUB_U); }

// shunting_yard converts an infix token array into an RPN token array.
// It returns the number of tokens written into output.
int shunting_yard(const int64_t *input, const int size, int64_t *output) {
  int64_t stack[MAX_TOKENS];
  int stack_top = 0;
  int out_index = 0;

  for (int i = 0; i < size; ++i) {
    int64_t token = input[i];

    // In this design, numbers are nonnegative.
    if (token >= 0) {
      output[out_index++] = token;
      continue;
    }

    // If token is a left parenthesis.
    if (token == OP(PAREN_L)) {
      stack[stack_top++] = token;
      continue;
    }

    // If token is a right parenthesis.
    if (token == OP(PAREN_R)) {
      while (stack_top > 0 && stack[stack_top - 1] != OP(PAREN_L)) {
        output[out_index++] = stack[--stack_top];
      }
      if (stack_top > 0 && stack[stack_top - 1] == OP(PAREN_L)) {
        --stack_top; // Discard the left parenthesis.
      }
      continue;
    }

    // Handle unary minus (neg) immediately after the operand.
    if (token == OP(SUB_U)) {
      stack[stack_top++] = token;
      continue;
    }

    // If it's a binary operator, compare precedence.
    while (stack_top > 0) {
      int64_t top = stack[stack_top - 1];
      if (top == OP(PAREN_L))
        break;

      if (precedence(token) < precedence(top)) {
        output[out_index++] = stack[--stack_top];
      } else {
        break;
      }
    }
    stack[stack_top++] = token;
  }

  // Pop any remaining operators off the stack.
  while (stack_top > 0) {
    output[out_index++] = stack[--stack_top];
  }

  return out_index;
}
