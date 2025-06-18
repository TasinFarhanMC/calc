#include <iostream>

extern "C" int add();

int main(int argc, char *argv[]) {
  std::cout << "Hello, World!: " << add() << std::endl;

  return 0;
}
