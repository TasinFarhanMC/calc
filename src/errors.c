#include "calc.h"

static const char *err_str[] = {
    "None",
    "Unknown Charecter",
    "Invalid Syntax",
    "Numeric Overflow",
};

const char *calc_get_err_str(CalcError err) { return err_str[err]; }
