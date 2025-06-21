# Calc

**Calc** is a mathematical expression parser and evaluator written in **C99**.
It is **freestanding** and can optionally interigate **C standard library** instead of custom implementations

## Cmake Interigation

Build and link using Cmake

```cmake
include(FetchContent)

FetchContent_Declare(
  calc
  GIT_REPOSITORY https://github.com/TasinFarhanMC/calc
  GIT_TAG        main
)

FetchContent_MakeAvailable(calc)

# Link against calc if it builds a library
# target_link_libraries(your_target PRIVATE calc)
```
    

## Build
``` shell
git clone https://github.com/TasinFarhanMC/calc
cd calc
cmake --preset release
cmake --preset release --build
cd lib
```
## Running Tests

To run tests, run the following command

```bash
./run.sh
```
or

```shell
cmake --preset debug -G Ninja -DCALC_TESTS=ON
cmake --preset debug --build
cd bin
ctest -V
```
## API Reference

### `CalcBufs calc_parse_ascii(const char *str, CalcU8 size);`

Parses a mathematical expression given as a ASCII string into a numeric and command buffer.

* **Parameters:**

  * `str`: Input string containing the mathematical expression.
  * `size`: Maximum number of characters to parse from `str`.

* **Returns:**
  A `CalcBufs` structure containing the buffers:
---

### `CalcCmds calc_gen_rpn(const CalcCmds cmds);`

Renranges the infix commands from a command buffer into Reverse Polish Notation (RPN).

* **Parameters:**

  * `cmds`: Command sequence in infix notation.

* **Returns:**
  A `CalcCmds` structure representing the RPN form of the expression.

---

### `CalcNum calc_eval_epn(const CalcBufs bufs);`

Evaluates the expression represented by the numeric and command buffers in postfix notation.

* **Parameters:**

  * `bufs`: Combined numeric and command buffers containing the expression in RPN.

* **Returns:**
  The resulting `CalcNum` numeric value of the evaluation.

---

## Example Usage

```c
#include "calc.h"

int main() {
    const char *expr = "3 + 5 - 2";
    CalcBufs parsed = calc_parse_ascii(expr, strlen(expr));
    CalcCmds rpn = calc_gen_rpn(parsed.cmds);
    CalcNum result = calc_eval_epn((CalcBufs){parsed.nums, rpn});
    // Access result.val depending on your configuration (fixed or float)
    return 0;
}
```
