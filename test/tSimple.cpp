#include "ilp.hpp"
#include "ilp.hpp" // make sure multiple include is ok
#include "gtest/gtest.h"
#include <cassert>

TEST(Simple, First) { ASSERT_EQ(ILP::foo(), 10); }

TEST(Simple, Ilp) {
  const auto N = 1UL;
  for (auto k = 0UL; k < N; ++k) {
    auto res = ILP::ilp(300000000UL);
    ASSERT_EQ(res, 3921015525972184403UL);
  }
}

TEST(Simple, Sum) {
  auto u1 = 100ULL, u2 = 21ULL;
  // Try rvalues
  ASSERT_EQ(ILP::sum(1), 1);
  ASSERT_EQ(ILP::sum(1, 2), 3);
  ASSERT_EQ(ILP::sum(1, 2, 3), 6);

  // Try lvalues
  ASSERT_EQ(ILP::sum(u1), u1);
  ASSERT_EQ(ILP::sum(u1, u2), u1 + u2);
  ASSERT_EQ(ILP::sum(u1, u2, u2), u1 + u2 + u2);

  // Mixed types
  ASSERT_EQ(ILP::sum(1.0, 2, 3UL), 6.0);
}
