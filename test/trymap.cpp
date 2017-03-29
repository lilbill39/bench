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
  it.key = 1;
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

  std::vector<int> vals({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
  std::for_each(vals.begin(), vals.end(), [&m](int i) { m.insert(2*i, i + 1); });
  std::for_each(vals.begin(), vals.end(), [&m](int i) {
    ASSERT_TRUE(m.lookup(2*i));
    ASSERT_FALSE(m.lookup(-i));
  });
}

TEST(rymap, stringKeys)
{
  ry::map<std::string, int> m;
  ASSERT_FALSE(m.lookup("foobar"));

  std::vector<std::pair<std::string,int>> vals = {{"hello",0}, {"why",1}, {"",2}, {"again",4}};
  ptrdiff_t idx(0);

  std::for_each(vals.begin(), vals.end(),
                [&vals, &m, &idx](const std::pair<std::string, int> &val) {
                  m.insert(val.first, val.second);
                  std::for_each(vals.begin(), vals.begin() + idx + 1,
                                [&m](const std::pair<std::string, int> &v) {
                                  auto it = m.lookup(v.first);
                                  ASSERT_TRUE(it);
                                  ASSERT_EQ(it.key, v.first);
                                  ASSERT_EQ(it.val, v.second);
                                });
                  std::for_each(vals.begin() + idx + 1, vals.end(),
                                [&m](const std::pair<std::string, int> &v) {
                                  auto it = m.lookup(v.first);
                                  ASSERT_FALSE(it);
                                });
                  ++idx;
                });

  m.insert("why",3);
  auto it = m.lookup("why");
  ASSERT_TRUE(it);
  ASSERT_EQ(it.key, "why");
  ASSERT_EQ(it.val, 3);
}
