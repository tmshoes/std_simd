#include <experimental/simd>

void sin_1(float* a) {
    auto x = std::experimental::simd<float>(a, std::experimental::element_aligned_tag{});
    x = std::experimental::sin(x);
    x.copy_to(a, std::experimental::element_aligned_tag{});
}
