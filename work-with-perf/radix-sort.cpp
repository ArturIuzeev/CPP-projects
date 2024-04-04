#include <benchmark/benchmark.h>

#include <cstdint>
#include <limits>
#include <memory>
#include <random>
#include <vector>

namespace {

std::vector<uint64_t> random_vector(size_t size) {
  std::vector<uint64_t> result;
  result.resize(size);

  std::mt19937 rng(std::mt19937::default_seed);
  std::uniform_int_distribution<uint64_t> element_dist;
  for (auto& e : result) {
    e = element_dist(rng);
  }

  return result;
}

size_t count_zero_bits(const std::vector<uint64_t>& input, size_t bit) {
  size_t count = 0;
  for (auto e : input) {
    count += (1 - (e >> bit)) & 1u;
  }
  return count;
}

static const int s = 1u << 24;
static std::vector<uint64_t> buffer(s);

// DO NOT change the algorithm
void radix_sort(std::vector<uint64_t>& input) {
  for (size_t bit = 0; bit < std::numeric_limits<uint64_t>::digits; ++bit) {
    auto count = count_zero_bits(input, bit);

    size_t i = 0;
    size_t j = count;
    for (auto e : input) {
      if ((e >> bit) & 1) {
        buffer[j++] = e;
      } else {
        buffer[i++] = e;
      }
    }

    input.swap(buffer);
  }
}

void bm_radix_sort(benchmark::State& state) {
  auto input = random_vector(s);
  auto copy = input;

  for (auto _ : state) {
    copy = input;
    radix_sort(copy);

    benchmark::DoNotOptimize(copy);
  }

  if (!std::is_sorted(copy.begin(), copy.end())) {
    state.SkipWithError("Sort is not sorting");
  }
}

} // namespace

BENCHMARK(bm_radix_sort)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
