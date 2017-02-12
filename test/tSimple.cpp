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
  ASSERT_EQ(ILP::sum(1), 1);
  ASSERT_EQ(ILP::sum(1, 2), 3);
  ASSERT_EQ(ILP::sum(1, 2, 3), 6);

  ASSERT_EQ(ILP::sum(1.0, 2, 3UL), 6.0);
}
