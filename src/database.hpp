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
#include <memory>
#include <type_traits>
#include "whim_framework.hpp"
namespace whimap
{
    class row
    {
    };
    template <typename T>
    using limit_operation = std::function<T(const T&, const T&)>;
    class column
    {
    public:
        using float_type = fp32;
        using simd_type  = fp32v;

    private:
        size_t     size;
        simd_type* data;

    public:
        column(float_type* ps, size_t s);

        auto min() const -> float_type;
        auto max() const -> float_type;
    };
    class table
    {
    };
} // namespace whimap
