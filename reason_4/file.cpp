
#include <benchmark/benchmark.h>
#include <experimental/simd>
#include <vector>

void loop_2(std::vector<int> &a) {
    for (auto &e : a) {
        e = (e & 1) == 0 ? e + e : e; 
    }
}

void loop_1(std::vector<int> &a) {
    using lanes_t = std::experimental::simd<int>;
    const std::size_t N = a.size();

    std::size_t i = 0;

    for (; i + lanes_t::size() <= N; i += lanes_t::size()) {
        lanes_t v(&a[i], std::experimental::element_aligned);
        auto even = (v & 1) == 0;
        std::experimental::where(even, v) = v + v;
        v.copy_to(&a[i], std::experimental::element_aligned);
    }

    for (; i < N; ++i) {
        int a_i = a[i];
        a[i] = ((a_i & 1) == 0) ? (a_i + a_i) : a_i;
    }
}


void BM_0(benchmark::State &s) {
	std::vector<int> a(1024);
	for (int i = 0; i < 1024; i++) a[i] = i;
	for (auto _ : s) {
		std::vector<int> b = a;
		benchmark::DoNotOptimize(&b);
	}
}

void BM_1(benchmark::State &s) {
	std::vector<int> a(1024);
	for (int i = 0; i < 1024; i++) a[i] = i;
	for (auto _ : s) {
		std::vector<int> b = a;
		benchmark::DoNotOptimize(&b);
		loop_1(b);
		benchmark::DoNotOptimize(&b);
	}
}

void BM_2(benchmark::State &s) {
	std::vector<int> a(1024);
	for (int i = 0; i < 1024; i++) a[i] = i;
	for (auto _ : s) {
		std::vector<int> b = a;
		benchmark::DoNotOptimize(&b);
		loop_2(b);
		benchmark::DoNotOptimize(&b);
	}
}

BENCHMARK(BM_0);
BENCHMARK(BM_1);
BENCHMARK(BM_2);

BENCHMARK_MAIN();
