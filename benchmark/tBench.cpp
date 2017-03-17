#include "ilp.hpp"
#include <benchmark/benchmark.h>
#include <vector>
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

BENCHMARK_MAIN();
