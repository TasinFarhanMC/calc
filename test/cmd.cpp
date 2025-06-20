#include <calc.h>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]) {
  auto str = "+";
  auto bufs = calc_parse_ascii(str, strlen(str));

  std::cout << "STR: " << str << std::endl;
  std::cout << "Buf NUM size: " << (int)bufs.nums.size << std::endl;
  for (int i = 0; i < bufs.nums.size; i++) { std::cout << "NUM: " << (int)bufs.nums.data[i].val << std::endl; }

  std::cout << "Buf CMD size: " << (int)bufs.cmds.size << std::endl;
  for (int i = 0; i < bufs.cmds.size; i++) { std::cout << "CMD: " << (int)bufs.cmds.data[i] << std::endl; }

  return 0;
}
