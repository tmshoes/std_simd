#include <benchmark/benchmark.h>
#include <experimental/simd>
#include <cmath>

#include <cstdint>

__attribute((noinline)) void f_1(float* a) {
    auto x = std::experimental::simd<float>(a, std::experimental::element_aligned_tag{});
    x = std::experimental::sqrt(x);
    x *= x;
    x.copy_to(a, std::experimental::element_aligned_tag{});
}

__attribute((noinline)) void f_2(float* a) {
    for (uint64_t i = 0; i < 8; i++) {
	float x = a[i];
	x = std::sqrt(x);
	x *= x;
        a[i] = x;
    }
}

void BM_1(benchmark::State &s) {

    std::array<float, 8> a = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,};

    for(auto _ : s) {
        auto b = a;
        benchmark::DoNotOptimize(&b);
        f_1(b.data());
        benchmark::DoNotOptimize(&b);
    }

}
void BM_2(benchmark::State &s) {

    std::array<float, 8> a = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,};

    for(auto _ : s) {
        auto b = a;
        benchmark::DoNotOptimize(&b);
        f_2(b.data());
        benchmark::DoNotOptimize(&b);
    }

}

BENCHMARK(BM_1);
BENCHMARK(BM_2);

BENCHMARK_MAIN();
