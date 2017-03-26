#include "ilp.hpp"
#include "rymap.hpp"
#include <benchmark/benchmark.h>
#include <vector>
#include <unordered_map>
void bm_ilp(benchmark::State &aState) {
  while (aState.KeepRunning()) {
    auto res = ILP::ilp(3000UL);
    benchmark::DoNotOptimize(res);
    benchmark::ClobberMemory();
  }
}

BENCHMARK(bm_ilp);

void bm_small_function(benchmark::State &aState) {
  const size_t N = 10000U;
  while (aState.KeepRunning()) {
    for (size_t k = 0; k < N; ++k) {
      auto f = ILP::make_small_function([]() { return 10; });
      auto n = f();
      benchmark::DoNotOptimize(n);
      benchmark::ClobberMemory();
    }
  }
}

BENCHMARK(bm_small_function);

void bm_function(benchmark::State &aState) {
  const size_t N = 10000U;
  while (aState.KeepRunning()) {
    for (size_t k = 0; k < N; ++k) {
      auto f = []() { return 10; };
      auto n = f();
      benchmark::DoNotOptimize(n);
      benchmark::ClobberMemory();
    }
  }
}

BENCHMARK(bm_function);

void bm_vector(benchmark::State &aState) {
  //const size_t N = 10000U;
  while (aState.KeepRunning()) {
    std::vector<double> v;
    v.reserve(6);
    v.push_back(1.234);
    v.push_back(2.234);
    v.push_back(3.234);
    v.push_back(3.234);
    v.push_back(3.234);
    v.push_back(3.234);        
    benchmark::DoNotOptimize(v);
    benchmark::ClobberMemory();
  }
}

BENCHMARK(bm_vector);

#define NUM 10000U;
void bm_rymap_insert(benchmark::State &aState) {
  const size_t N = NUM;
  ry::map<size_t,size_t> m;

  while (aState.KeepRunning()) {
    for (size_t i = 0U; i < N; ++i) {
      m.insert(i, i + 1);
      benchmark::DoNotOptimize(m);
      benchmark::ClobberMemory();
    }
  }
}

BENCHMARK(bm_rymap_insert);

void bm_unordered_map_insert(benchmark::State &aState) {
  const size_t N = NUM;
  std::unordered_map<size_t,size_t> m;

  while (aState.KeepRunning()) {
    for (size_t i = 0U; i < N; ++i) {
      m.insert(std::make_pair(i, i + 1));
      benchmark::DoNotOptimize(m);
      benchmark::ClobberMemory();
    }
  }
}

BENCHMARK(bm_unordered_map_insert);

void bm_rymap_lookup(benchmark::State &aState) {
  const size_t N = NUM;
  ry::map<size_t,size_t> m;

  // for (size_t i = 0U; i < 10U; ++i) {
  //   m.insert(2*i, i + 1);
  //   benchmark::DoNotOptimize(m);
  //   benchmark::ClobberMemory();
  // }
  while (aState.KeepRunning()) {
    for (size_t i = 0U; i < N; ++i) {
      auto v = m.lookup(2*i);
      benchmark::DoNotOptimize(v);
      benchmark::ClobberMemory();
    }
  }
}

BENCHMARK(bm_rymap_lookup);

void bm_unordered_map_lookup(benchmark::State &aState) {
  const size_t N = NUM;
  std::unordered_map<size_t,size_t> m;

  // for (size_t i = 0U; i < 10U; ++i) {
  //   m.insert(std::make_pair(2*i, i+1));
  //   benchmark::DoNotOptimize(m);
  //   benchmark::ClobberMemory();
  // }
  while (aState.KeepRunning()) {
    for (size_t i = 0U; i < N; ++i) {
      auto v = m.find(2*i);
      benchmark::DoNotOptimize(v);
      benchmark::ClobberMemory();
    }
  }
}

BENCHMARK(bm_unordered_map_lookup);

BENCHMARK_MAIN();
