#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CALC_USE_STDLIB
#include <stdint.h>
typedef uint8_t CalcU8;
typedef uint16_t CalcU16;
typedef uint32_t CalcU32;
typedef uintmax_t CalcUMax;

typedef int32_t Calc32;
#else
typedef unsigned char CalcU8;
typedef unsigned short CalcU16;
typedef unsigned int CalcU32;
typedef unsigned long long CalcUMax;

typedef int Calc32;
#endif

#ifdef CALC_NUM_FIXED

#if CALC_NUM_WIDTH == 32
typedef CalcU32 CalcUSize;
typedef CalcU16 CalcUHalf;

typedef Calc32 CalcSize;

#elif CALC_NUM_WIDTH == 64
#ifdef CALC_USE_STDLIB
typedef uint64_t CalcU64;
typedef int64_t Calc64;
#else
typedef unsigned long long CalcU64;
typedef long long Calc64;
#endif

typedef CalcU64 CalcUSize;
typedef CalcU32 CalcUHalf;

typedef Calc64 CalcSize;

#else
#error "Unsupported CALC_NUM_WIDTH for fixed numeric"
#endif

typedef union {
  struct {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    CalcUHalf low;
    CalcUHalf high;
#else
    CalcUHalf high;
    CalcUHalf low;
#endif
  };

  CalcUSize val;
} CalcNum;

#elif defined(CALC_NUM_FLOAT)

#if CALC_NUM_WIDTH == 32
typedef float CalcF32;
typedef CalcF32 CalcFSize;
#define CALC_FSIZE_INF (*(const CalcFSize *)&(const unsigned long long) {0x7F800000}) // THX windows

#elif CALC_NUM_WIDTH == 64
typedef double CalcF64;
typedef CalcF64 CalcFSize;
#define CALC_FSIZE_INF (*(const CalcFSize *)&(const unsigned long long) {0x7FF0000000000000ULL}) // THX windows

#elif CALC_NUM_WIDTH == 80
typedef long double CalcF80;
typedef CalcF80 CalcFSize;

#if defined(_MSC_VER)
#error "MSVC does not support 80-bit long double"
#else
#define CALC_FSIZE_INF ((CalcFSize)1 / (CalcFSize)0) // Windows will FUCK
#endif

#else
#error "Unsupported CALC_NUM_WIDTH for floating numeric"
#endif

typedef struct {
  CalcFSize val;
} CalcNum;
#else
#error "Invalid numeric type specified"
#endif

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
  CalcU16 size;
} CalcNums;

typedef struct {
  CalcCmd *data;
  CalcU16 size;
} CalcCmds;

typedef struct {
  CalcNums nums;
  CalcCmds cmds;
} CalcBufs;

typedef enum {
  CALC_ERR_NONE,
  CALC_ERR_UNKNOWN_CHAR,
  CALC_ERR_INV_FRAC,
  CALC_ERR_NUM_OVERFLOW,
  CALC_ERR_DIV_ZERO,
  CALC_ERR_STACK_OVERFLOW,
  CALC_ERR_INV_SYNTAX,
} CalcError;

typedef struct {
  CalcError err;
  CalcNum ok;
} CalcNumResult;

typedef struct {
  CalcError err;
  CalcCmds ok;
} CalcCmdsResult;

typedef struct {
  CalcError err;
  CalcBufs ok;
} CalcBufsResult;

CalcBufsResult calc_parse_ascii(const char *str, CalcU16 size);
CalcCmdsResult calc_gen_rpn(const CalcCmds cmds);
CalcNumResult calc_eval_rpn(const CalcBufs bufs);
CalcNumResult calc_mul_num(const CalcNum a, const CalcNum b);
CalcNumResult calc_div_num(const CalcNum a, const CalcNum b);
const char *calc_get_err_str(CalcError err);

#ifdef __cplusplus
}
#endif
