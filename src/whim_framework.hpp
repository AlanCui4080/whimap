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
#include <experimental/simd>
namespace whimap
{  
    using fp32 = float;
    using fp32v = std::experimental::native_simd<fp32>;
    static_assert(sizeof(float)*8 == 32, "width of fp32 is not satisfied 32-bits");
    using fp64 = double;
    using fp64v = std::experimental::native_simd<fp64>;
    static_assert(sizeof(double)*8 == 64, "width of fp64 is not satisfied 64-bits");
    using fp128 = long double;
    using fp128v = std::experimental::native_simd<fp128>;
    static_assert(sizeof(long double)*8 == 128, "width of fp128 is not satisfied 128-bits");

    #define align_cache alignas(32);
} // namespace whimap

