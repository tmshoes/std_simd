
#include <experimental/simd>
#include <stdfloat>

void func(std::float16_t *a) {
    auto x = std::experimental::simd<std::float16_t>(a, std::experimental::element_aligned_tag{});
    auto m = x > std::float16_t(324.324);
    std::experimental::where(m, x) = x + x;
    x.copy_to(a, std::experimental::element_aligned_tag{});
}

