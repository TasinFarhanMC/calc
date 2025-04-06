#include <stdint.h>

#define RIG 0
#define LEF 1
#define NEG 2
#define ADD 3
#define SUB 4
#define MUL 5
#define DIV 6

#define OP_FIRST (INT64_C(1) << 63)
#define OP_FIRST_U (UINT64_C(1) << 63)
#define OP_LAST (OP_FIRST + DIV)

#define OP(TOKEN) (TOKEN + OP_FIRST)
#define NUM(VALUE) (VALUE << 32)

#define IS_NUM(VALUE) (VALUE < OP_FIRST_U)
#define IS_OP(VALUE) (VALUE >= OP_FIRST_U)

#define FRACTIONAL_BITS 32

#define MAX_TOKENS 1024
