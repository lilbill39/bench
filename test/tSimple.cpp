#include "ilp.hpp"
#include "ilp.hpp"
#include "gtest/gtest.h"
#include <cassert>

TEST(Simple, First) { ASSERT_EQ(foo(), 10); }

TEST(Simple, Ilp) {
  const auto N = 1UL;
  for (auto k = 0UL; k < N; ++k) {
    auto res = ilp(300000000UL);
    ASSERT_GT(res, 0);
  }
}
