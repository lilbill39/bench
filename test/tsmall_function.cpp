#include "ilp.hpp"
#include "gtest/gtest.h"

namespace {
  double localNullary() { return 1.2; };

}

TEST(small_function, nullary) {
  using namespace ILP;

  // Return by value
  auto f = make_small_function([]() { return 10; });
  f();
  ASSERT_EQ(f(), 10);

  // Fcn pointer
  auto fp = make_small_function(localNullary);
  ASSERT_EQ(fp(), localNullary());
  
  // Void
  int n = 1;
  auto fv = make_small_function([&n]() { ++n; });
  fv();
  ASSERT_EQ(n, 2);
}

TEST(small_function, unary) {
  using namespace ILP;

  // Pass POD by value
  auto f = make_small_function([](int n) { return ++n; });
  ASSERT_EQ(f(1), 2);

  // Pass POD by reference
  int n = 1;
  auto fr = make_small_function([](int &x) { return ++x; });
  ASSERT_EQ(fr(n), 2);
  ASSERT_EQ(n, 2);
}
