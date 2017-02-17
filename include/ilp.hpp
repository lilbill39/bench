#pragma once
#include <stddef.h>
#include <utility>
namespace ILP {
int foo();
size_t ilp(size_t x);

namespace {
// Traits class to resolve sum type using decltype
template <typename... Args>
struct sum_traits;

template <typename T>
struct sum_traits<T> {
  typedef T type;
};

// Need to drop lvalue reference
template <typename T>
struct sum_traits<T &> {
  typedef T type;
};

template <typename T, typename... Args>
struct sum_traits<T, Args...> {
  typedef decltype(T() + typename sum_traits<Args...>::type()) type;
};

// Need to drop lvalue reference
template <typename T, typename... Args>
struct sum_traits<T &, Args...> {
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

/// Small wrapper around a callable. Used to locally store a callable.
///
/// See make_small_function for simpler construction
template <typename Callable>
class small_function {
 private:
  Callable F;

 public:
  small_function(Callable aF) : F(aF) {}

  template <typename... Args>
  auto operator()(Args &&... args)
      -> decltype(this->F(std::forward<Args>(args)...)) {
    return F(std::forward<Args>(args)...);
  }
};

/// Helper for small_function. Uses type deduction to construct easily.
template <typename Callable>
small_function<Callable> make_small_function(Callable aF) {
  return small_function<Callable>(aF);
}

} // namespace ILP
