#include <stdint.h>

#define NOP 0
#define LEF 1
#define RIG 2
#define NEG 3
#define ADD 4
#define SUB 5
#define MUL 6
#define DIV 7
#define T 8

#define OP_COUNT 8
#define OP_FIRST (INT64_C(1) << 63)
#define IS_OP(VALUE) (VALUE & OP_FIRST)

#define OP(TOKEN) (TOKEN + OP_FIRST)
#define NUM(VALUE) (VALUE << 32)

#define FRACTIONAL_BITS 32

#define MAX_TOKENS 1024
