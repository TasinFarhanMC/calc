#include "calc.h"

#ifdef CALC_NUM_FIXED
#if CALC_NUM_FIXED == 64
#elif CALC_NUM_WIDTH == 32
#else
#endif
#elif defined(CALC_NUM_FLOAT)
#else
#endif

CalcNumResult calc_mul_num(const CalcNum a, const CalcNum b);
CalcNumResult calc_div_num(const CalcNum a, const CalcNum b);
