#include <cstdint>
#include <cmath>

void sin_2(float* a) {
    for (uint64_t i = 0; i < 8; i++) {
        a[i] = std::sin(a[i]);
    }
}
