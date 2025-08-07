#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CALC_INT
typedef CALC_INT CalcNum;
#define CALC_SHIFT (sizeof(CALC_INT) << 2)

typedef CALC_INT CalcInt;
typedef unsigned CALC_INT CalcUInt;
#elif defined(CALC_FLOAT)
typedef CALC_FLOAT CalcNum;
typedef int CalcInt;
typedef unsigned CalcUInt;
#else
#error "NEITHER CALC_INT NOR CALC_FLOAT DEFINED"
#endif // def CALC_INT

typedef enum {
  CALC_CMD_LOAD,
  CALC_CMD_ADD,
  CALC_CMD_SUB,
  CALC_CMD_NEG,
  CALC_CMD_MUL,
  CALC_CMD_DIV,
  CALC_CMD_L_BRACE,
  CALC_CMD_R_BRACE,
} CalcCmd;

typedef struct {
  CalcNum *data;
  CalcInt size;
  CalcInt cap;
} CalcNums;

typedef struct {
  CalcCmd *data;
  CalcInt size;
  CalcInt cap;
} CalcCmds;

typedef enum {
  CALC_ERR_NONE,
  CALC_ERR_UNKNOWN_CHAR,
  CALC_ERR_INV_FRAC,
  CALC_ERR_NUM_OVERFLOW,
  CALC_ERR_DIV_ZERO,
  CALC_ERR_STACK_OVERFLOW,
  CALC_ERR_INV_SYNTAX,
} CalcErr;

#define CALC_NUMS_FROM_ARR()
#define CALC_CMDS_FROM_ARR()

CalcErr calc_parse_ascii(const char *str, CalcUInt size, CalcNums *nums, CalcCmds *cmds);
CalcErr calc_parse_ascii_to(const char *str, CalcUInt pos, char c, CalcNums *nums, CalcCmds *cmds);

CalcErr calc_to_rpn(CalcCmds cmds);
CalcErr calc_gen_rpn(CalcCmds cmds, CalcCmds *rpn);

CalcNum calc_evaluate_rpn(CalcCmds cmds, CalcNums nums, CalcErr *err); // Nullable Err

CalcNum calc_evaluate_ascii(const char *str, CalcUInt size, CalcErr *err);           // Nullable Err
CalcNum calc_evaluate_ascii_to(const char *str, CalcUInt pos, char c, CalcErr *err); // Nullable Err

CalcErr calc_mul_num(const CalcNum a, const CalcNum b);
CalcErr calc_div_num(const CalcNum a, const CalcNum b);

const char *calc_debug_str(CalcErr err);

#ifdef __cplusplus
}
#endif
