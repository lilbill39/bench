#include "ilp.hpp"
#include "gtest/gtest.h"

TEST(small_function, nullary) {
  using namespace ILP;

  // Return by value
  auto f = make_small_function([](){return 10;});
  f();
  ASSERT_EQ(f(), 10);

  // Void
  int n = 1;
  auto fv = make_small_function([&n](){ ++n;});
  fv();
  ASSERT_EQ(n,2);
}
