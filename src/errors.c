#include "calc.h"

static const char *err_str[] = {
    "None",
    "Unknown Charecter",
    "Invalid Fraction",
    "Numeric Overflow",
};

const char *calc_get_err_str(CalcError err) { return err_str[err]; }
