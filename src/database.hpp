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
#include <functional>
#include <optional>

#include "whim_framework.hpp"
#include "whim_exception.hpp"
#include "simd-utility.hpp"
namespace whimap
{
    template <typename S> class basic_column_base
    {
    };
    template <typename T, typename S>
    class basic_hashed_column : public basic_column_base<S>
    {
    public:
        using value_type  = T;
        using scala_type  = S;
        using vector_type = expvec<scala_type>;

    private:
        vector_type mdata;

    public:
        // get a empty column not impl yet :(
        basic_hashed_column() = delete;
        // create a column filled with vec XD
        basic_hashed_column(std::vector<value_type>& vec)
        {
            auto aligned_ptr = simd::AllocateAligned<scala_type>(vec.size());
            for (size_t i = 0; i < vec.size(); i++)
            {
                aligned_ptr[i] =
                    static_cast<scala_type>(std::hash<value_type>{}(vec[i]));
            }
            mdata = std::move(vector_type(std::move(aligned_ptr), vec.size()));
        }
    };
    // template <typename T, typename S>
    // class basic_bitmap_column : public basic_hashed_column<T, S>
    // {
    // public:
    //     using value_type    = T;
    //     using scala_type    = S;
    //     using vector_type   = expvec<scala_type>;
    //     using bitarray_type = expvec<vector_type>;

    // private:
    //     bitarray_type m_bitarray;

    // public:
    //     // get a empty column not impl yet :(
    //     basic_bitmap_column() = delete;
    //     // create a column filled with vec XD
    //     basic_bitmap_column(const std::vector<value_type>& vec);
    // };
    template <typename T>
    using hashed_column = basic_hashed_column<T, simd::float16_t>;
} // namespace whimap
