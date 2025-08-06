#include "calc.h"

#ifdef CALC_NUM_FIXED
CalcNumResult calc_mul_num(const CalcNum a, const CalcNum b) {
  CalcNumResult result = {CALC_ERR_NONE, {0}};

  // Multiply parts (all 16-bit * 16-bit = 32-bit)
  CalcUSize low_low = (CalcUSize)a.low * (CalcUSize)b.low;
  CalcUSize low_high = (CalcUSize)a.low * (CalcUSize)b.high;
  CalcUSize high_low = (CalcUSize)a.high * (CalcUSize)b.low;
  CalcUSize high_high = (CalcUSize)a.high * (CalcUSize)b.high;

  // Combine cross terms
  CalcUSize mid = low_high + high_low;
  CalcUSize carry = 0;
  if (mid < low_high) carry = 1; // overflow in mid sum

  // Compose low part
  CalcUSize low = low_low + (mid << 16);
  if (low < low_low) carry++;

  // Compose high part
  CalcUSize high = high_high + (mid >> 16) + carry;

  // If high != 0, overflow occurred (assuming 32-bit CalcUSize)
  if (high != 0) {
    result.err = CALC_ERR_NUM_OVERFLOW;
    result.ok.val = (CalcUSize)-1; // saturate max value
    return result;
  }

  result.ok.val = low;
  return result;
}

CalcNumResult calc_div_num(const CalcNum a, const CalcNum b) {}

#elif defined(CALC_NUM_FLOAT)

CalcNumResult calc_mul_num(const CalcNum a, const CalcNum b) {
  CalcFSize res = a.val * b.val;
  if (res == CALC_FSIZE_INF) { return (CalcNumResult) {CALC_ERR_NUM_OVERFLOW}; }

  return (CalcNumResult) {CALC_ERR_NONE, {res}};
}

CalcNumResult calc_div_num(const CalcNum a, const CalcNum b) {
  if (a.val == 0 || b.val == 0) { return (CalcNumResult) {CALC_ERR_DIV_ZERO}; }
  CalcFSize res = a.val / b.val;

  if (res == CALC_FSIZE_INF) { return (CalcNumResult) {CALC_ERR_NUM_OVERFLOW}; }
  return (CalcNumResult) {CALC_ERR_NONE, {res}};
}

#endif
