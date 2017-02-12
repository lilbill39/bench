#pragma once
#include <stddef.h>
#include <utility>
namespace ILP {
int foo();
size_t ilp(size_t x);

namespace {
template <typename... Args>
struct sum_traits;

template <typename T>
struct sum_traits<T> {
  typedef T type;
};

template <typename T, typename... Args>
struct sum_traits<T, Args...> {
  typedef decltype(T() + typename sum_traits<Args...>::type()) type;
};
} // namespace
template <typename T>
T sum(T &&a) {
  return a;
}

template <typename T, typename... Args>
typename sum_traits<T, Args...>::type sum(T &&a, Args &&... args) {
  return a + ILP::sum(std::forward<Args>(args)...);
}
} // namespace ILP
