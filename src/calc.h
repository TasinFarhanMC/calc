#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef unsigned char calc_u8;
typedef unsigned short calc_u16;
typedef unsigned int calc_u32;

#if CALC_NUM_TYPE == X64
typedef unsigned long calc_u64;
typedef calc_u64 calc_size;

typedef union {
  struct {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    calc_u32 low;
    calc_u32 high;
#else
    u32 high;
    u32 low;
#endif
  };
  calc_size val;
} Num;
#elif CALC_NUM_TYPE == X32
typedef calc_u32 calc_size;

typedef union {
  struct {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    calc_u16 low;
    calc_u16 high;
#else
    calc_u16 high;
    calc_u16 low;
#endif
  };
  calc_u16 val;
} Fx;
#endif

typedef enum {
  CMD_ADD,
  CMD_SUB,
} Cmd;

typedef struct {
  Num *data;
  calc_u8 size;
} Nums;

typedef struct {
  Cmd *data;
  calc_u8 size;
} Cmds;

typedef struct {
  Nums nums;
  Cmds cmds;
} Bufs;

Bufs parse_string(const char *str, calc_u8 size);
Cmds generate_rpn(const Cmds nums);
Num evaluate_rpn(const Bufs bufs);

#ifdef __cplusplus
}
#endif // __cplusplus
