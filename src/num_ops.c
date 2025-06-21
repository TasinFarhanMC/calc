#include "calc.h"

#ifdef CALC_NUM_FIXED
#if CALC_NUM_FIXED == 64
#elif CALC_NUM_WIDTH == 32
#else
#endif
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
