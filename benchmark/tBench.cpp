#include "ilp.hpp"
#include <benchmark/benchmark.h>
void bm_ilp(benchmark::State &aState) {
  while (aState.KeepRunning()) {
    auto res = ILP::ilp(3000UL);
    benchmark::DoNotOptimize(res);
    benchmark::ClobberMemory();
  }
}

BENCHMARK(bm_ilp);
BENCHMARK_MAIN();
