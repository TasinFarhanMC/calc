#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CALC_USE_STDLIB
#include <stdint.h>
typedef uint8_t CalcU8;
typedef uint16_t CalcU16;
typedef uint32_t CalcU32;
#else
typedef unsigned char CalcU8;
typedef unsigned short CalcU16;
typedef unsigned int CalcU32;
#endif

#ifdef CALC_NUM_FIXED

#if CALC_NUM_WIDTH == 32
typedef CalcU32 CalcUSize;
typedef CalcU16 CalcUHalf;

#elif CALC_NUM_WIDTH == 64

#ifdef CALC_USE_STDLIB
typedef uint64_t CalcU64;
#else
typedef unsigned long long CalcU64;
#endif

typedef CalcU64 CalcUSize;
typedef CalcU32 CalcUHalf;

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

#elif CALC_NUM_WIDTH == 64
typedef double CalcF64;
typedef CalcF64 CalcFSize;

#elif CALC_NUM_WIDTH == 80
typedef long double CalcF80;
typedef CalcF80 CalcFSize;

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
} CalcCmd;

typedef struct {
  CalcNum *data;
  CalcU8 size;
} CalcNums;

typedef struct {
  CalcCmd *data;
  CalcU8 size;
} CalcCmds;

typedef struct {
  CalcNums nums;
  CalcCmds cmds;
} CalcBufs;

CalcBufs calc_parse_ascii(const char *str, CalcU8 size);
CalcCmds calc_gen_rpn(const CalcCmds nums);
CalcNum calc_eval_epn(const CalcBufs bufs);

#ifdef __cplusplus
}
#endif
