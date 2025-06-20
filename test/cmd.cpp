#include <calc.h>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]) {
  std::string s;
  std::getline(std::cin, s);
  const char *str = s.c_str();
  auto res = calc_parse_ascii(str, strlen(str));
  auto bufs = res.ok;

  if (res.err != CALC_ERR_NONE) {
    std::cout << "Err: " << calc_get_err_str(res.err) << std::endl;
    return 0;
  }

  std::cout << "STR: " << str << std::endl;
  std::cout << "Buf NUM size: " << (int)bufs.nums.size << std::endl;
  for (int i = 0; i < bufs.nums.size; i++) { std::cout << "NUM: " << bufs.nums.data[i].val << std::endl; }

  std::cout << "Buf CMD size: " << (int)bufs.cmds.size << std::endl;
  for (int i = 0; i < bufs.cmds.size; i++) { std::cout << "CMD: " << bufs.cmds.data[i] << std::endl; }

  return 0;
}
