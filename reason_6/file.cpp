
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

