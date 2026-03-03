
# Top reasons why you should use std::simd

## Reason 1

I am not lying when I say that most of our day job is spent doomscrolling on TikTok nowadays. Yet for some reason, managers don't believe that this time is spent "productively". 
The C++ standard committee is working hard to improve the DX for every C++ developer. The idea they came up with is called long compile times. That way, you can tell the PM that you are productive, and the code just needs to be compiled.

```
#include <experimental/simd>

void sin_1(float* a) {
    auto x = std::experimental::simd<float>(a, std::experimental::element_aligned_tag{});
    x = std::experimental::sin(x);
    x.copy_to(a, std::experimental::element_aligned_tag{});
}
```

```
$ hyperfine -N "g++ -O3 -ffast-math -march=znver5 reason_1/file_1.cpp -std=c++26 -c"
Benchmark 1: g++ -O3 -ffast-math -march=znver5 reason_1/file_1.cpp -std=c++26 -c
  Time (mean ± σ):      1.577 s ±  0.017 s    [User: 1.529 s, System: 0.042 s]
  Range (min … max):    1.559 s …  1.611 s    10 runs
```

```
#include <cstdint>
#include <cmath>

void sin_2(float* a) {
    for (uint64_t i = 0; i < 8; i++) {
        a[i] = std::sin(a[i]);
    }
}
```

```
$ hyperfine -N "g++ -O3 -ffast-math -march=znver5 reason_1/file_2.cpp -std=c++26 -c"
Benchmark 1: g++ -O3 -ffast-math -march=znver5 reason_1/file_2.cpp -std=c++26 -c
  Time (mean ± σ):     162.9 ms ±  14.0 ms    [User: 151.2 ms, System: 10.9 ms]
  Range (min … max):   154.9 ms … 200.1 ms    14 runs
```

With std::simd, you are guaranteed to multiply your compilation times.
I bet you are like: "Only one second?" But trust me, as your codebase grows, your compile times will as well.

## Reason 2

Nowadays, we have to convince our investors and customers that we need to work continuously on software to justify charging each month.
The C++ standard committee is working hard to enable C++ to adapt to modern software development requirements.
The solution is quite simple: just make your software slow initially so you can make cool graphs later showing how that hard work pays off.
std::simd is specifically designed to be slower than boomer loops initially to have headroom for optimisations in the future.


```
$ clang++ reason_2/file.cpp -DNDEBUG -std=c++26 -march=native -O3 -ffast-math -lbenchmark -pthread -fveclib=libmvec
```

```
------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
BM_1_mean               5.00 ns         4.99 ns            5
BM_1_median             5.00 ns         4.99 ns            5
BM_1_stddev            0.004 ns        0.004 ns            5
BM_1_cv                 0.08 %          0.08 %             5
BM_2_mean               3.78 ns         3.77 ns            5
BM_2_median             3.78 ns         3.77 ns            5
BM_2_stddev            0.015 ns        0.015 ns            5
BM_2_cv                 0.40 %          0.40 %             5
```

## Reason 3

Let's review the development of screen resolutions back when C++98 was published. Monitor resolutions were around 1024x768. But nowadays 1920x1080 is more common, and my laptop even has 4K.
As resolutions increased, C++ error messages couldn't keep pace. The C++ standard committee has designated this problem a high priority, as it leaves a lot of potential on the table for squeezing everything out of your hardware.
std::simd should hopefully address this issue.

```

#include <experimental/simd>
#include <stdfloat>

void func(std::float16_t *a) {
    auto x = std::experimental::simd<std::float16_t>(a, std::experimental::element_aligned_tag{});
    auto m = x > std::float16_t(324.324);
    std::experimental::where(m, x) = x + x;
    x.copy_to(a, std::experimental::element_aligned_tag{});
}

```

Results in:

```
In file included from /usr/include/c++/15.2.1/experimental/simd:74,
                 from reason_3/file.cpp:2:
/usr/include/c++/15.2.1/experimental/bits/simd.h: In instantiation of 'constexpr _TW std::experimental::parallelism_v2::__andnot(_TW, _TW) [with _TW = _SimdWrapper<_Float16, 8, void>]':
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:845:28:   required from 'static constexpr std::experimental::parallelism_v2::_SimdWrapper<_Tp, _Np> std::experimental::parallelism_v2::_CommonImplX86::_S_blend(std::experimental::parallelism_v2::_SimdWrapper<decltype (__int_for_sizeof<sizeof (_Tp)>()), _Np>, std::experimental::parallelism_v2::_SimdWrapper<_Tp, _Np>, std::experimental::parallelism_v2::_SimdWrapper<_Tp, _Np>) [with _Tp = _Float16; long unsigned int _Np = 8; decltype (__int_for_sizeof<sizeof (_Tp)>()) = short int]'
  845 |           auto __r = __or(__andnot(__kk, __at0), __and(__kk, __at1));
      |                           ~~~~~~~~^~~~~~~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd_builtin.h:2361:33:   required from 'static constexpr void std::experimental::parallelism_v2::_SimdImplBuiltin<_Abi, <template-parameter-1-2> >::_S_masked_assign(std::experimental::parallelism_v2::_SimdWrapper<_K, _Np>, std::experimental::parallelism_v2::_SimdWrapper<_Tp, _Np>&, std::__type_identity_t<std::experimental::parallelism_v2::_SimdWrapper<_Tp, _Np> >) [with _Tp = _Float16; _K = short int; long unsigned int _Np = 8; _Abi = std::experimental::parallelism_v2::simd_abi::_VecBuiltin<16>; <template-parameter-1-2> = std::experimental::parallelism_v2::__detail::_MachineFlagsTemplate<221055, 10>; std::__type_identity_t<std::experimental::parallelism_v2::_SimdWrapper<_Tp, _Np> > = std::__type_identity<std::experimental::parallelism_v2::_SimdWrapper<_Float16, 8, void> >::type]'
 2361 |           __lhs = _CommonImpl::_S_blend(__k, __lhs, __rhs);
      |                   ~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:3654:25:   required from 'void std::experimental::parallelism_v2::where_expression<_M, _Tp>::operator=(_Up&&) && [with _Up = std::experimental::parallelism_v2::simd<_Float16>; _M = std::experimental::parallelism_v2::simd_mask<_Float16, std::experimental::parallelism_v2::simd_abi::_VecBuiltin<16> >; _Tp = std::experimental::parallelism_v2::simd<_Float16>]'
 3654 |         _Impl::_S_masked_assign(__data(_M_k), __data(_M_value),
      |         ~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 3655 |                                 __to_value_type_or_member_type<_Tp>(
      |                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 3656 |                                   static_cast<_Up&&>(__x)));
      |                                   ~~~~~~~~~~~~~~~~~~~~~~~~~
reason_3/file.cpp:8:42:   required from here
    8 |     std::experimental::where(m, x) = x + x;
      |                                          ^
/usr/include/c++/15.2.1/experimental/bits/simd.h:2050:47: error: no match for call to '(const std::experimental::parallelism_v2::<unnamed struct>) (const __vector(8) _Float16&, const __vector(8) _Float16&)'
 2050 |                 const auto __r = _S_x86_andnot(__ai, __bi);
      |                                  ~~~~~~~~~~~~~^~~~~~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:1973:1: note: there are 9 candidates
 1973 | {
      | ^
/usr/include/c++/15.2.1/experimental/bits/simd.h:1975:3: note: candidate 1: '__v4sf std::experimental::parallelism_v2::<unnamed struct>::operator()(__v4sf, __v4sf) const'
 1975 |   operator()(__v4sf __a, __v4sf __b) const noexcept
      |   ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:1975:21: note: no known conversion for argument 1 from 'const __vector(8) _Float16' to '__v4sf'
 1975 |   operator()(__v4sf __a, __v4sf __b) const noexcept
      |              ~~~~~~~^~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:1979:3: note: candidate 2: '__v2df std::experimental::parallelism_v2::<unnamed struct>::operator()(__v2df, __v2df) const'
 1979 |   operator()(__v2df __a, __v2df __b) const noexcept
      |   ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:1979:21: note: no known conversion for argument 1 from 'const __vector(8) _Float16' to '__v2df'
 1979 |   operator()(__v2df __a, __v2df __b) const noexcept
      |              ~~~~~~~^~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:1983:3: note: candidate 3: '__v2di std::experimental::parallelism_v2::<unnamed struct>::operator()(__v2di, __v2di) const'
 1983 |   operator()(__v2di __a, __v2di __b) const noexcept
      |   ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:1983:21: note: no known conversion for argument 1 from 'const __vector(8) _Float16' to '__v2di'
 1983 |   operator()(__v2di __a, __v2di __b) const noexcept
      |              ~~~~~~~^~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:1987:3: note: candidate 4: '__v8sf std::experimental::parallelism_v2::<unnamed struct>::operator()(__v8sf, __v8sf) const'
 1987 |   operator()(__v8sf __a, __v8sf __b) const noexcept
      |   ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:1987:21: note: no known conversion for argument 1 from 'const __vector(8) _Float16' to '__v8sf'
 1987 |   operator()(__v8sf __a, __v8sf __b) const noexcept
      |              ~~~~~~~^~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:1991:3: note: candidate 5: '__v4df std::experimental::parallelism_v2::<unnamed struct>::operator()(__v4df, __v4df) const'
 1991 |   operator()(__v4df __a, __v4df __b) const noexcept
      |   ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:1991:21: note: no known conversion for argument 1 from 'const __vector(8) _Float16' to '__v4df'
 1991 |   operator()(__v4df __a, __v4df __b) const noexcept
      |              ~~~~~~~^~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:1995:3: note: candidate 6: '__v4di std::experimental::parallelism_v2::<unnamed struct>::operator()(__v4di, __v4di) const'
 1995 |   operator()(__v4di __a, __v4di __b) const noexcept
      |   ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:1995:21: note: no known conversion for argument 1 from 'const __vector(8) _Float16' to '__v4di'
 1995 |   operator()(__v4di __a, __v4di __b) const noexcept
      |              ~~~~~~~^~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:2006:3: note: candidate 7: '__v16sf std::experimental::parallelism_v2::<unnamed struct>::operator()(__v16sf, __v16sf) const'
 2006 |   operator()(__v16sf __a, __v16sf __b) const noexcept
      |   ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:2006:22: note: no known conversion for argument 1 from 'const __vector(8) _Float16' to '__v16sf'
 2006 |   operator()(__v16sf __a, __v16sf __b) const noexcept
      |              ~~~~~~~~^~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:2017:3: note: candidate 8: '__v8df std::experimental::parallelism_v2::<unnamed struct>::operator()(__v8df, __v8df) const'
 2017 |   operator()(__v8df __a, __v8df __b) const noexcept
      |   ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:2017:21: note: no known conversion for argument 1 from 'const __vector(8) _Float16' to '__v8df'
 2017 |   operator()(__v8df __a, __v8df __b) const noexcept
      |              ~~~~~~~^~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:2028:3: note: candidate 9: '__v8di std::experimental::parallelism_v2::<unnamed struct>::operator()(__v8di, __v8di) const'
 2028 |   operator()(__v8di __a, __v8di __b) const noexcept
      |   ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:2028:21: note: no known conversion for argument 1 from 'const __vector(8) _Float16' to '__v8di'
 2028 |   operator()(__v8di __a, __v8di __b) const noexcept
      |              ~~~~~~~^~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:2054:26: error: no type named 'type' in 'using _TVT = std::conditional_t<true, std::experimental::parallelism_v2::_SimdWrapper<_Float16, 8, void>, std::experimental::parallelism_v2::_VectorTraitsImpl<std::experimental::parallelism_v2::_SimdWrapper<_Float16, 8, void>, void> >' {aka 'using std::conditional<true, std::experimental::parallelism_v2::_SimdWrapper<_Float16, 8, void>, std::experimental::parallelism_v2::_VectorTraitsImpl<std::experimental::parallelism_v2::_SimdWrapper<_Float16, 8, void>, void> >::type = struct std::experimental::parallelism_v2::_SimdWrapper<_Float16, 8, void>'}
 2054 |                   return reinterpret_cast<typename _TVT::type>(__r);
      |                          ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
In file included from /usr/include/c++/15.2.1/experimental/simd:80:
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h: In instantiation of 'static _Tp std::experimental::parallelism_v2::_CommonImplX86::_S_blend_intrin(_Tp, _Tp, _Tp) [with _Tp = __vector(8) _Float16]':
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:863:28:   required from 'static constexpr std::experimental::parallelism_v2::_SimdWrapper<_Tp, _Np> std::experimental::parallelism_v2::_CommonImplX86::_S_blend(std::experimental::parallelism_v2::_SimdWrapper<decltype (__int_for_sizeof<sizeof (_Tp)>()), _Np>, std::experimental::parallelism_v2::_SimdWrapper<_Tp, _Np>, std::experimental::parallelism_v2::_SimdWrapper<_Tp, _Np>) [with _Tp = _Float16; long unsigned int _Np = 8; decltype (__int_for_sizeof<sizeof (_Tp)>()) = short int]'
  863 |             return _S_blend_intrin(__to_intrin(__kk), __to_intrin(__at0),
      |                    ~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  864 |                                    __to_intrin(__at1));
      |                                    ~~~~~~~~~~~~~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd_builtin.h:2361:33:   required from 'static constexpr void std::experimental::parallelism_v2::_SimdImplBuiltin<_Abi, <template-parameter-1-2> >::_S_masked_assign(std::experimental::parallelism_v2::_SimdWrapper<_K, _Np>, std::experimental::parallelism_v2::_SimdWrapper<_Tp, _Np>&, std::__type_identity_t<std::experimental::parallelism_v2::_SimdWrapper<_Tp, _Np> >) [with _Tp = _Float16; _K = short int; long unsigned int _Np = 8; _Abi = std::experimental::parallelism_v2::simd_abi::_VecBuiltin<16>; <template-parameter-1-2> = std::experimental::parallelism_v2::__detail::_MachineFlagsTemplate<221055, 10>; std::__type_identity_t<std::experimental::parallelism_v2::_SimdWrapper<_Tp, _Np> > = std::__type_identity<std::experimental::parallelism_v2::_SimdWrapper<_Float16, 8, void> >::type]'
 2361 |           __lhs = _CommonImpl::_S_blend(__k, __lhs, __rhs);
      |                   ~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd.h:3654:25:   required from 'void std::experimental::parallelism_v2::where_expression<_M, _Tp>::operator=(_Up&&) && [with _Up = std::experimental::parallelism_v2::simd<_Float16>; _M = std::experimental::parallelism_v2::simd_mask<_Float16, std::experimental::parallelism_v2::simd_abi::_VecBuiltin<16> >; _Tp = std::experimental::parallelism_v2::simd<_Float16>]'
 3654 |         _Impl::_S_masked_assign(__data(_M_k), __data(_M_value),
      |         ~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 3655 |                                 __to_value_type_or_member_type<_Tp>(
      |                                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 3656 |                                   static_cast<_Up&&>(__x)));
      |                                   ~~~~~~~~~~~~~~~~~~~~~~~~~
reason_3/file.cpp:8:42:   required from here
    8 |     std::experimental::where(m, x) = x + x;
      |                                          ^
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:802:20: error: no match for call to '(const std::experimental::parallelism_v2::_CommonImplX86::_S_blend_intrin<__vector(8) _Float16>(__vector(8) _Float16, __vector(8) _Float16, __vector(8) _Float16)::<unnamed struct>) (__vector(8) _Float16&, __vector(8) _Float16&, __vector(8) _Float16&)'
  802 |       return __eval(__a, __b, __k);
      |              ~~~~~~^~~~~~~~~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:758:7: note: there are 6 candidates
  758 |       {
      |       ^
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:759:40: note: candidate 1: '__m128 std::experimental::parallelism_v2::_CommonImplX86::_S_blend_intrin(_Tp, _Tp, _Tp)::<unnamed struct>::operator()(__m128, __m128, __m128) const [with _Tp = __vector(8) _Float16; __m128 = __m128]'
  759 |         _GLIBCXX_SIMD_INTRINSIC __m128 operator()(__m128 __a, __m128 __b,
      |                                        ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:759:58: note: no known conversion for argument 1 from '__vector(8) _Float16' to '__m128'
  759 |         _GLIBCXX_SIMD_INTRINSIC __m128 operator()(__m128 __a, __m128 __b,
      |                                                   ~~~~~~~^~~
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:764:41: note: candidate 2: '__m128d std::experimental::parallelism_v2::_CommonImplX86::_S_blend_intrin(_Tp, _Tp, _Tp)::<unnamed struct>::operator()(__m128d, __m128d, __m128d) const [with _Tp = __vector(8) _Float16; __m128d = __m128d]'
  764 |         _GLIBCXX_SIMD_INTRINSIC __m128d operator()(__m128d __a, __m128d __b,
      |                                         ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:764:60: note: no known conversion for argument 1 from '__vector(8) _Float16' to '__m128d'
  764 |         _GLIBCXX_SIMD_INTRINSIC __m128d operator()(__m128d __a, __m128d __b,
      |                                                    ~~~~~~~~^~~
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:769:41: note: candidate 3: '__m128i std::experimental::parallelism_v2::_CommonImplX86::_S_blend_intrin(_Tp, _Tp, _Tp)::<unnamed struct>::operator()(__m128i, __m128i, __m128i) const [with _Tp = __vector(8) _Float16; __m128i = __m128i]'
  769 |         _GLIBCXX_SIMD_INTRINSIC __m128i operator()(__m128i __a, __m128i __b,
      |                                         ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:769:60: note: no known conversion for argument 1 from '__vector(8) _Float16' to '__m128i'
  769 |         _GLIBCXX_SIMD_INTRINSIC __m128i operator()(__m128i __a, __m128i __b,
      |                                                    ~~~~~~~~^~~
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:777:40: note: candidate 4: '__m256 std::experimental::parallelism_v2::_CommonImplX86::_S_blend_intrin(_Tp, _Tp, _Tp)::<unnamed struct>::operator()(__m256, __m256, __m256) const [with _Tp = __vector(8) _Float16; __m256 = __m256]'
  777 |         _GLIBCXX_SIMD_INTRINSIC __m256 operator()(__m256 __a, __m256 __b,
      |                                        ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:777:58: note: no known conversion for argument 1 from '__vector(8) _Float16' to '__m256'
  777 |         _GLIBCXX_SIMD_INTRINSIC __m256 operator()(__m256 __a, __m256 __b,
      |                                                   ~~~~~~~^~~
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:782:41: note: candidate 5: '__m256d std::experimental::parallelism_v2::_CommonImplX86::_S_blend_intrin(_Tp, _Tp, _Tp)::<unnamed struct>::operator()(__m256d, __m256d, __m256d) const [with _Tp = __vector(8) _Float16; __m256d = __m256d]'
  782 |         _GLIBCXX_SIMD_INTRINSIC __m256d operator()(__m256d __a, __m256d __b,
      |                                         ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:782:60: note: no known conversion for argument 1 from '__vector(8) _Float16' to '__m256d'
  782 |         _GLIBCXX_SIMD_INTRINSIC __m256d operator()(__m256d __a, __m256d __b,
      |                                                    ~~~~~~~~^~~
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:787:41: note: candidate 6: '__m256i std::experimental::parallelism_v2::_CommonImplX86::_S_blend_intrin(_Tp, _Tp, _Tp)::<unnamed struct>::operator()(__m256i, __m256i, __m256i) const [with _Tp = __vector(8) _Float16; __m256i = __m256i]'
  787 |         _GLIBCXX_SIMD_INTRINSIC __m256i operator()(__m256i __a, __m256i __b,
      |                                         ^~~~~~~~
/usr/include/c++/15.2.1/experimental/bits/simd_x86.h:787:60: note: no known conversion for argument 1 from '__vector(8) _Float16' to '__m256i'
  787 |         _GLIBCXX_SIMD_INTRINSIC __m256i operator()(__m256i __a, __m256i __b,
      |                                                    ~~~~~~~~^~~
```

This error is so cryptic that it also adds the possibility of adding C++ error messages as a post-quantum encryption scheme in future releases.

## Reason 4

```
void loop_2(std::vector<int> &a) {
    for (auto &e : a) {
        e = (e & 1) == 0 ? e + e : e;
    }
}
```

Looking at the boomer loop, we can see all the problems of pre-C++26. We have raw, unsafe memory access, and the reader needs to understand pointers and other low-level bit-manipulation concepts that beginners often struggle with.
std::simd aims to make performant code easy to read and reason about.
The sad thing is that, for some reason, modern languages focus on designing them so that correct code is "clean". Yet I usually work with incorrect code. I don't care about how the correct code looks.
All I want is to identify bad code as soon as possible. The result is that modern languages create code that looks clean but behaves unintuitively and is fragile.
The C++ standard committee positions itself against the mainstream with std::simd, which merges readability with high-level constructs in an ideal manner.

```
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
```


```
$ clang++ reason_4/file.cpp -DNDEBUG -std=c++26 -march=native -O3 -ffast-math -lbenchmark -pthread -fveclib=libmvec
```

```
------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
BM_0_mean               44.8 ns         44.7 ns            5
BM_0_median             44.7 ns         44.6 ns            5
BM_0_stddev            0.182 ns        0.175 ns            5
BM_0_cv                 0.41 %          0.39 %             5
BM_1_mean                133 ns          133 ns            5
BM_1_median              133 ns          133 ns            5
BM_1_stddev            0.356 ns        0.357 ns            5
BM_1_cv                 0.27 %          0.27 %             5
BM_2_mean               83.5 ns         83.4 ns            5
BM_2_median             77.9 ns         77.7 ns            5
BM_2_stddev             7.93 ns         7.92 ns            5
BM_2_cv                 9.49 %          9.50 %             5
```

Again, love to see the optimisation potential of std::simd in comparison to the boomer loop in the future. This is thanks to the fact that the default vector width is, in fact, not the machine vector width.
All this time, haters have wished that C++ would be fixed, mostly through the type system, by making alignment part of the type and by providing a proper system to define aliasing behaviour.
They hoped that C++ makes writing performant code easy by fixing further gotchas like that "int8_t + int8_t = int32_t" which is the primary reason behind X "C/C++ sucks write asm instead" shitposts.
But the haters wont simply understand that this behaviour is crucial for C++ because otherwise uint16_t(0xFFFF) * uint16_t(0xFFFF) would not be UB and thus the "unsignd interger overflow does wrap around" rule would not have any exceptions making C++ fell less C++ like.
But to any hater hoping that C++ comes up with a proper SIMD as a first-class experience through SIMD control-flow mechanisms on the language level. Or even first-class support for portable heterogeneous compute (i.e., Vulkan SPIRV as a target):
As it turns out, you should just have waited longer and watched the C++ standard committee cook, and look how much they did cook.

## Reason 5

I now have to switch to a more serious tone. In the era of AI, we see how people are affected when machines try to take their jobs away.
And the same happens in C++ as well. Because what are compilers if not computer algebra systems?
Take this example:

```
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
```

And the generated assembly in reason_5/file.asm

```
_Z3f_1Pf:
        vsqrtps xmm0, oword [rdi]                       ; 0000BB60 _ C5 F8: 51. 07
        vmulps  xmm0, xmm0, xmm0                        ; 0000BB64 _ C5 F8: 59. C0
        vmovups oword [rdi], xmm0                       ; 0000BB68 _ C5 F8: 11. 07
        ret                                             ; 0000BB6C _ C3
```

```
_Z3f_2Pf:
        ret                                             ; 0000BC20 _ C3
```

The compiler removes all the code from the function in the boomer loop because the sqrt cancels with the ^2. This work is taken over by the machine, even if there are perfectly capable mathematicians who would like to handcraft the perfect code.
Only a psychopath without empathy would not stand up against the machines here. As does the C++ standard committee, which stands with humankind by preferring libraries over language features.

## Reason 6

For most programmers, the act of producing code is what gives them joy. Many ask me if there is a danger to std::simd being portable.
Instead of writing the same code multiple times => more fun, they fear they only need to write the code once => less fun.
Many fear this will impact morale in the long term.

Let's look at the code from reason 4.

```
Disassembly of section .text:

0000000000000000 <_Z6loop_2RSt6vectorIiSaIiEE>:
   0: a9400001  ldp x1, x0, [x0]
   4: eb01001f  cmp x0, x1
   8: 540001e0  b.eq 44 <_Z6loop_2RSt6vectorIiSaIiEE+0x44>  // b.none
   c: d1001002  sub x2, x0, #0x4
  10: d2800000  mov x0, #0x0                    // #0
  14: cb010042  sub x2, x2, x1
  18: d342fc42  lsr x2, x2, #2
  1c: 91000442  add x2, x2, #0x1
  20: 25a21fe7  whilelo p7.s, xzr, x2
  24: a5405c3f  ld1w {z31.s}, p7/z, [x1, x0, lsl #2]
  28: 04bf03fe  add z30.s, z31.s, z31.s
  2c: 0580001f  and z31.s, z31.s, #0x1
  30: 25809fe7  cmpeq p7.s, p7/z, z31.s, #0
  34: e5405c3e  st1w {z30.s}, p7, [x1, x0, lsl #2]
  38: 04b0e3e0  incw x0
  3c: 25a21c07  whilelo p7.s, x0, x2
  40: 54ffff21  b.ne 24 <_Z6loop_2RSt6vectorIiSaIiEE+0x24>  // b.any
  44: d65f03c0  ret

0000000000000048 <_Z6loop_1RSt6vectorIiSaIiEE>:
  48: a9400c04  ldp x4, x3, [x0]
  4c: cb040063  sub x3, x3, x4
  50: 9342fc65  asr x5, x3, #2
  54: f100307f  cmp x3, #0xc
  58: 540004c9  b.ls f0 <_Z6loop_1RSt6vectorIiSaIiEE+0xa8>  // b.plast
  5c: 3dc0009f  ldr q31, [x4]
  60: d2800202  mov x2, #0x10                   // #16
  64: d2800101  mov x1, #0x8                    // #8
  68: d2800086  mov x6, #0x4                    // #4
  6c: 4ebf1ffe  mov v30.16b, v31.16b
  70: 4ebf87fd  add v29.4s, v31.4s, v31.4s
  74: 0580001e  and z30.s, z30.s, #0x1
  78: 4ea09bde  cmeq v30.4s, v30.4s, #0
  7c: 6efe1ffd  bif v29.16b, v31.16b, v30.16b
  80: 3d80009d  str q29, [x4]
  84: f100707f  cmp x3, #0x1c
  88: 540001e9  b.ls c4 <_Z6loop_1RSt6vectorIiSaIiEE+0x7c>  // b.plast
  8c: f9400003  ldr x3, [x0]
  90: aa0103e6  mov x6, x1
  94: 91001021  add x1, x1, #0x4
  98: 8b020064  add x4, x3, x2
  9c: 3ce2687c  ldr q28, [x3, x2]
  a0: 91004042  add x2, x2, #0x10
  a4: 4ebc1f9b  mov v27.16b, v28.16b
  a8: 4ebc879a  add v26.4s, v28.4s, v28.4s
  ac: 0580001b  and z27.s, z27.s, #0x1
  b0: 4ea09b7b  cmeq v27.4s, v27.4s, #0
  b4: 6efb1f9a  bif v26.16b, v28.16b, v27.16b
  b8: 3d80009a  str q26, [x4]
  bc: eb0100bf  cmp x5, x1
  c0: 54fffe62  b.cs 8c <_Z6loop_1RSt6vectorIiSaIiEE+0x44>  // b.hs, b.nlast
  c4: eb0600bf  cmp x5, x6
  c8: 54000129  b.ls ec <_Z6loop_1RSt6vectorIiSaIiEE+0xa4>  // b.plast
  cc: f9400000  ldr x0, [x0]
  d0: cb0600a5  sub x5, x5, x6
  d4: 25a50fe7  whilelo p7.s, wzr, w5
  d8: a5465c19  ld1w {z25.s}, p7/z, [x0, x6, lsl #2]
  dc: 04b90338  add z24.s, z25.s, z25.s
  e0: 05800019  and z25.s, z25.s, #0x1
  e4: 25809f27  cmpeq p7.s, p7/z, z25.s, #0
  e8: e5465c18  st1w {z24.s}, p7, [x0, x6, lsl #2]
  ec: d65f03c0  ret
  f0: d2800006  mov x6, #0x0                    // #0
  f4: 17fffff4  b c4 <_Z6loop_1RSt6vectorIiSaIiEE+0x7c>
```

The solution is simple: std::simd is portable in the sense that it compiles on ARM, yet as you can see here, the assembly of the std::simd doesn't use SVE, the armv9 simd instructions, for its main loop. Yet the boomer loop does.
This is why writing std::simd is more fun than old boomer loops.

## Final remarks

I would love to play more with std::simd, but support for it is still early.
That means for a lot of problems, we're kind of forced to play around with boomer loops, for exampl we need to wait until std::complex for std::simd is implemented.

```
void loop_2(std::vector<std::complex<float>> &a) {
    for (auto &e : a) {
        e *= e;
    }
}
```

```
.L4:
        vmovups zmm1, ZMMWORD PTR [rax]
        vmovups zmm2, ZMMWORD PTR [rax+64]
        sub     rax, -128
        vmovaps zmm0, zmm1
        vpermt2ps       zmm1, zmm5, zmm2
        vpermt2ps       zmm0, zmm6, zmm2
        vmulps  zmm2, zmm0, zmm1
        vmulps  zmm1, zmm1, zmm1
        vfmsub132ps     zmm0, zmm1, zmm0
        vaddps  zmm2, zmm2, zmm2
        vmovaps zmm1, zmm0
        vpermt2ps       zmm0, zmm3, zmm2
        vpermt2ps       zmm1, zmm4, zmm2
        vmovups ZMMWORD PTR [rax-64], zmm0
        vmovups ZMMWORD PTR [rax-128], zmm1
        cmp     rdx, rax
        jne     .L4
```

But std::simd aligns C++ with the direction that many programming languages like Java, Rust & Go take. std::simd paves the way for the future of C++, where using the hardware as effectively as possible is key.
std::simd solves the problem that every architecture has its own cryptically named and non-portable wrapper around the clean and portable built-in + operator well.

```
__ai __attribute__((target("fullfp16,neon"))) float16x8_t vaddq_f16(float16x8_t __p0, float16x8_t __p1) {
  float16x8_t __ret;
  __ret = __p0 + __p1;
  return __ret;
}
```

```
static __inline __m512d __DEFAULT_FN_ATTRS512
_mm512_add_pd(__m512d __a, __m512d __b)
{
  return (__m512d)((__v8df)__a + (__v8df)__b);
}
```

