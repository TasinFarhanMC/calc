#include <calc.h>
#include <cassert>
#include <iostream>

int main(int argc, char *argv[]) {
  Bufs bufs = parse_string("12.25", 5);

  assert(bufs.nums.data[0].val == (calc_size)(12.25f * (1UL << 32)));

  return 0;
}
