#include <stdint.h>

#define PAREN_L 0
#define PAREN_R 1
#define MUL 2
#define ADD 3
#define SUB_U 4
#define SUB_B 5
#define DIV 7

#define OP_FIRST (INT64_C(1) << 63)
#define OP_LAST (OP_FIRST + DIV)
#define OP(TOKEN) (TOKEN + OP_FIRST)

#define FRACTIONAL_BITS 32
#define MAX_TOKENS 1024
