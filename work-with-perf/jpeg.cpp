#include <benchmark/benchmark.h>

#include <random>
#include <utility>
#include <vector>

namespace {

struct Pixel {
  int x;
  int y;
  int z;

  int operator[](int n) const {
    if (n == 0) {
      return x;
    } else if (n == 1) {
      return y;
    } else {
      return z;
    }
  }
};

using Picture = std::vector<std::vector<Pixel>>;

Picture random_picture() {
  static constexpr size_t HEIGHT = 600;
  static constexpr size_t WIDTH = 800;

  std::mt19937 rng(std::mt19937::default_seed);
  std::uniform_int_distribution<int> color_dist(0, 255);

  Picture picture;
  for (size_t i = 0; i < HEIGHT; i++) {
    picture.emplace_back();
    for (size_t j = 0; j < WIDTH; j++) {
      picture.back().emplace_back(
          std::move(Pixel{color_dist(rng), color_dist(rng), color_dist(rng)}));
    }
  }
  return picture;
}

Picture scale_down(const Picture& picture) {
  Picture scaled;

  for (size_t i = 0; i < picture.size(); i += 2) {
    scaled.emplace_back();
    for (size_t j = 0; j < picture[0].size(); j += 2) {
      auto avg_component = [&](size_t component_idx) {
        return (picture[i][j][component_idx] + picture[i + 1][j][component_idx] +
                picture[i][j + 1][component_idx] + picture[i + 1][j + 1][component_idx]) /
               4;
      };

      int red = avg_component(0);
      int blue = avg_component(1);
      int green = avg_component(2);
      scaled.back().emplace_back(std::move(Pixel{red, blue, green}));
    }
  }

  return scaled;
}

void bm_scale_components(benchmark::State& state) {
  const auto picture = random_picture();

  for (auto _ : state) {
    const auto copy = picture;
    benchmark::DoNotOptimize(scale_down(copy));
  }
}

} // namespace

BENCHMARK(bm_scale_components)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
