#include <stdint.h>
#include <stdio.h>

uint32_t q32_32_mul(uint32_t a, uint32_t b) {
  uint32_t a_hi = a >> 16;
  uint32_t a_lo = a & 0xFFFF;
  uint32_t b_hi = b >> 16;
  uint32_t b_lo = b & 0xFFFF;

  uint32_t p0 = (uint32_t)a_lo * b_lo; // low * low
  uint32_t p1 = (uint32_t)a_lo * b_hi; // low * high
  uint32_t p2 = (uint32_t)a_hi * b_lo; // high * low
  uint32_t p3 = (uint32_t)a_hi * b_hi; // high * high

  // Sum p1 + p2 safely with carry detection
  uint32_t mid = p1 + p2;
  uint32_t carry = (mid < p1); // detect overflow in mid

  mid += (p0 >> 16);
  if (mid < (p0 >> 16)) carry++;

  uint32_t result = (p3 << 16) + mid + (carry << 16);

  return result;
}

int main() {
  uint32_t a = (123456123.7892131 * (1 << 16));  // 123456.789 in Q32.16
  uint32_t b = (98765320.432121312 * (1 << 16)); // 98765.4321 in Q32.16
  uint32_t expected = ((uint64_t)a * b) >> 16;   // ~12192558172.76 Q32.16

  uint32_t result = q32_32_mul(a, b);

  printf("Expected %u\n", expected);
  printf("Found   %u\n", result);
  return 0;
}
