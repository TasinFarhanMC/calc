#include <stdint.h>
#include <stdio.h>
#include <token.h>

int lexer(const char *str, int64_t *buffer);
int shunting_yard(const int64_t *input, const int size, int64_t *output);
int64_t rpn(const int64_t *buffer, int size);

static const char *const op_token_names[] = {
    [RIG] = "(", [LEF] = ")", [MUL] = "*", [ADD] = "+", [NEG] = "(-)", [SUB] = "-", [DIV] = "/",
};

char get_str_buf[MAX_TOKENS];

char *get_str(int64_t token) {

  switch (token) {
  case OP(ADD):
    return "+";
  case OP(NEG):
    return "(-)";
  case OP(SUB):
    return "-";
  case OP(MUL):
    return "*";
  case OP(DIV):
    return "/";
  case OP(LEF):
    return "(";
  case OP(RIG):
    return ")";
  default:
    snprintf(get_str_buf, sizeof(get_str_buf), "%lf", (double)token / (1UL << 32));
    return get_str_buf;
  }
}

int main() {
  // Step 1: Get input
  char str[MAX_TOKENS];
  printf("Enter expression: ");
  fgets(str, sizeof(str), stdin);

  // Step 2: Tokenize the expression
  int64_t buffer[MAX_TOKENS];
  int buffer_size = lexer(str, buffer);

  printf("\nLexer Output (Tokens): ");
  for (int i = 0; i < buffer_size; i++) { printf("%s ", get_str(buffer[i])); }
  printf("\n");

  // Step 3: Run Shunting Yard Algorithm
  int64_t output[MAX_TOKENS];
  int output_size = shunting_yard(buffer, buffer_size, output);

  printf("\nShunting Yard Output (Postfix): ");
  for (int i = 0; i < output_size; i++) { printf("%s ", get_str(output[i])); }
  printf("\n");

  // Step 4: Evaluate RPN
  int64_t result = rpn(output, output_size);

  printf("\nRPN Evaluation Result: %lf\n", (double)result / (1UL << 32));

  // Step 5: Process result into integer and fractional parts
  int64_t int_part = result >> 32;

  // Get fractional part, make sure it's positive before rounding
  uint64_t frac_raw = (uint64_t)(result & 0xFFFFFFFFULL);

  // Scale to 4 decimal places with rounding (i.e., *10000 / 2^32 + 0.5)
  uint64_t frac_part = (frac_raw * 10000 + (1ULL << 31)) >> 32;

  // Step 6: Print final result
  printf("Result: %ld.%04lu\n", int_part, frac_part);

  return 0;
}
