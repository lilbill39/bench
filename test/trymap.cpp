#include "rymap.hpp"
// Check double inclusion
#include "rymap.hpp"
#include "gtest/gtest.h"
TEST(rymap, construct) {
  ry::map<int, int> m;
  // Empty lookup should be false
  ASSERT_FALSE(m.lookup(12));
  
  m.insert(1, 12);

  auto it = m.lookup(1);
  ASSERT_TRUE(it);
  ASSERT_EQ(it.key, 1);
  ASSERT_EQ(it.val, 12);

  m.insert(3, 17);
  ASSERT_TRUE(m.lookup(3));
  it = m.lookup(10);
  ASSERT_FALSE(it);

  m.insert(17, 7);
  auto it17 = m.lookup(17);
  ASSERT_TRUE(it17);
  ASSERT_EQ(it17.key, 17);
  ASSERT_EQ(it17.val, 7);

  auto it1again = m.lookup(1);
  ASSERT_TRUE(it1again);
  ASSERT_EQ(it1again.key, 1);
  ASSERT_EQ(it1again.val, 12);
}
