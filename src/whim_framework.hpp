// This file a part of WhimAP
// Copyright (C) 2023 WhimAP Authors

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#pragma once
#include <concepts>
#include <type_traits>
#include <experimental/simd>
namespace whimap
{
    namespace simd
    {
        using std::experimental::max;
        using std::experimental::min;
    } // namespace simd

    constexpr auto width_vmm = 256;
    using fp32               = float;
    using fp32v              = std::experimental::native_simd<fp32>;
    static_assert(sizeof(float) * 8 == 32,
                  "width of fp32 is not satisfied 32-bits");
    using fp64  = double;
    using fp64v = std::experimental::native_simd<fp64>;
    static_assert(sizeof(double) * 8 == 64,
                  "width of fp64 is not satisfied 64-bits");
    using fp128  = long double;
    using fp128v = std::experimental::native_simd<fp128>;
    static_assert(sizeof(long double) * 8 == 128,
                  "width of fp128 is not satisfied 128-bits");
    template <std::floating_point F, std::integral T>
    constexpr auto is_aligned_simd(T v) noexcept -> bool
    {
        return !(v % std::experimental::native_simd<F>::size());
    }
    template <std::floating_point F, std::integral T>
    constexpr auto to_simd_size(T v) -> size_t
    {
        return v / std::experimental::native_simd<F>::size();
    }

#define align_cache alignas(32)
#define align_simd(T)                              \
    alignas(std::experimental::memory_alignment_v< \
            std::experimental::native_simd<T>>)
} // namespace whimap
