#pragma once

#include <calc.h>
#include <cmath>
#include <cstring>
#include <iostream>

static void dump_bufs(const CalcBufs &bufs) {
  std::cerr << "Nums (" << (int)bufs.nums.size << "): ";
  for (int i = 0; i < bufs.nums.size; ++i) std::cerr << bufs.nums.data[i].val << " ";
  std::cerr << "\nCmds (" << (int)bufs.cmds.size << "): ";
  for (int i = 0; i < bufs.cmds.size; ++i) std::cerr << bufs.cmds.data[i] << " ";
  std::cerr << std::endl;
}

static void dump_bufs(const CalcCmds &bufs) {
  std::cerr << "Cmds (" << (int)bufs.size << "): ";
  for (int i = 0; i < bufs.size; ++i) std::cerr << bufs.data[i] << " ";
  std::cerr << std::endl;
}

static void dump_bufs(const CalcNum &bufs) { std::cerr << "Res :" << bufs.val << std::endl; }

#define TEST_CASE(DESC, COND)                                                                                                                          \
  do {                                                                                                                                                 \
    if (!(COND)) {                                                                                                                                     \
      dump_bufs(result.ok);                                                                                                                            \
      std::cerr << "Group: " << name << std::endl;                                                                                                     \
      std::cerr << "❌ Test failed: " << (DESC) << std::endl;                                                                                          \
      return 1;                                                                                                                                        \
    }                                                                                                                                                  \
  } while (0)

#define START_TEST() int main() {

#define END_TEST()                                                                                                                                     \
  std::cout << "✅ All tests passed!\n";                                                                                                               \
  return 0;                                                                                                                                            \
  }
