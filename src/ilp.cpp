#include "ilp.hpp"
int foo() { return 10; }
size_t ilp(size_t x) {
  size_t a(0), b(0);
  for (size_t k = 0; k < x; ++k) {
    a = a * k;
    b = a * k + 1;
    a = b + 1;
  }
  return b;
}
