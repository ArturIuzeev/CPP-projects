#include <benchmark/benchmark.h>

#include <cstdint>
#include <cstdlib>
#include <random>

namespace {

// Computes PI using Monte Carlo method
// DO NOT change the algorithm
struct MonteCarlo {
  int64_t total = 0;
  int64_t inside_circle = 0;

  MonteCarlo() {
    gen.seed(42);
  }

  void operator()() {
    int x = gen() % SQUARE_SIZE;
    int y = gen() % SQUARE_SIZE;

    ++total;
    if (x * x + y * y < SQUARE_SIZE * SQUARE_SIZE) {
      ++inside_circle;
    }
  }

  double get_pi() const {
    return 4 * static_cast<double>(inside_circle) / static_cast<double>(total);
  }

private:
  std::mt19937 gen;
  static constexpr size_t SQUARE_SIZE = 1000;
};

void bm_compute_pi(benchmark::State& state) {
  MonteCarlo mc;

  for (auto _ : state) {
    mc();
  }

  state.counters["pi"] = mc.get_pi() / state.threads();
}

} // namespace

BENCHMARK(bm_compute_pi)->Threads(2);

BENCHMARK_MAIN();
