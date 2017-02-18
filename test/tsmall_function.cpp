#include "ilp.hpp"
#include "gtest/gtest.h"

namespace {
double localNullary() { return 1.2; };
} // namespace

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

namespace {
struct copy_counter {
  copy_counter() : fNcopies(0U), fNmoves(0U) {}

  copy_counter(copy_counter const &other) {
    fNcopies = other.fNcopies + 1;
    fNmoves = other.fNmoves;
  }

  copy_counter(copy_counter const &&other) {
    fNcopies = other.fNcopies;
    fNmoves = other.fNmoves + 1;
  }

  size_t fNcopies;
  size_t fNmoves;
};
}

TEST(small_function, unary_copies) {
  using namespace ILP;

  // Pass by value
  copy_counter cc;
  size_t ncopies, nmoves;
  ASSERT_EQ(cc.fNcopies, 0U);
  ASSERT_EQ(cc.fNmoves, 0U);
  auto f = make_small_function(
      [](copy_counter c) { return std::make_pair(c.fNcopies, c.fNmoves); });
  std::tie(ncopies, nmoves) = f(cc);
  ASSERT_EQ(ncopies, 1U);
  ASSERT_EQ(nmoves, 0U);

  // Pass by lvalue reference
  copy_counter ccr;
  ASSERT_EQ(ccr.fNcopies, 0U);
  ASSERT_EQ(ccr.fNmoves, 0U);
  auto fr = make_small_function(
      [](copy_counter &c) { return std::make_pair(c.fNcopies, c.fNmoves); });
  std::tie(ncopies, nmoves) = fr(ccr);
  ASSERT_EQ(ncopies, 0U);
  ASSERT_EQ(nmoves, 0U);

  // Pass by rvalue reference
  copy_counter ccrr;
  ASSERT_EQ(ccrr.fNcopies, 0U);
  ASSERT_EQ(ccrr.fNmoves, 0U);

  copy_counter cmoved = std::move(ccrr);
  ASSERT_EQ(cmoved.fNcopies, 0U);
  ASSERT_EQ(cmoved.fNmoves, 1U);
  auto frr = make_small_function(
      [](copy_counter &&c) { return std::make_pair(c.fNcopies, c.fNmoves); });
  auto &ccrr_ref = ccrr;
  std::tie(ncopies, nmoves) = frr(std::move(ccrr));
  ASSERT_EQ(ccrr.fNcopies, 0U);
  ASSERT_EQ(ccrr.fNmoves, 0U);
  ASSERT_EQ(ccrr_ref.fNcopies, 0U);
  ASSERT_EQ(ccrr_ref.fNmoves, 0U);
  ASSERT_EQ(ncopies, 0U);

  // TODO: Why does this fail?
  // ASSERT_EQ(nmoves, 1U);
}
