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
#include "database.hpp"
#include <stdexcept>
#include <limits>
#include <mutex>
using namespace whimap;
auto column<float>::min() -> value_type
{
    std::lock_guard lg(lock);
    simd_type vlim = std::numeric_limits<float_type>::quiet_NaN();
    for (size_t i = 0; i < size; i++)
    {
        vlim = std::move(simd::min(data[i], vlim));
    }
    float_type flim = std::numeric_limits<float_type>::quiet_NaN();
    for (size_t i = 0; i < simd_type::size(); i++)
    {
        flim = std::min(static_cast<float_type&&>(std::move(vlim[i])), flim);
    }
    return flim;
}
auto column<float>::max() -> value_type
{
    std::lock_guard lg(lock);
    simd_type vlim = std::numeric_limits<float_type>::quiet_NaN();
    for (size_t i = 0; i < size; i++)
    {
        vlim = std::move(simd::max(data[i], vlim));
    }
    float_type flim = std::numeric_limits<float_type>::quiet_NaN();
    for (size_t i = 0; i < simd_type::size(); i++)
    {
        flim = std::max(static_cast<float_type&&>(std::move(vlim[i])), flim);
    }
    return flim;
}
