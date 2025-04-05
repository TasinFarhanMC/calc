#include <stdio.h>
int main(void) {
  printf("Res: %lu\n", (1UL << 32) * (1UL << 32));
  return 0;
}
