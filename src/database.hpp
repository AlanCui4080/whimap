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
#include "transcation_impl.hpp"
namespace whimap
{
    class row
    {
    };
    /**
     * @brief the base class of every column
     * 
     * @headerfile database.hpp
     * @tparam T value type
     */
    template <typename T> class basic_column
    {
    public:
        using value_type = T;
        using float_type = fp32;
        using simd_type  = fp32v;

    public:
        /**
         * @brief get summary of all elements in this column
         * 
         * @return value_type 
         * 
         * @date 2023/8/31
         * @author AlanCui4080
         */
        virtual value_type sum() = 0;
        virtual value_type min() = 0;
        virtual value_type max() = 0;

    public:
        rwlock     lock;
        size_t     size;
        simd_type* data;
    /**
     * @brief Construct a new basic column object
     * 
     * @param ps pointer to a **aligned** by align_simd(T) array 
     * @param s size of elements which is multiplies of simd_type::size()
     */
        basic_column(float_type* ps, size_t s)
            : size(s)
            , data(reinterpret_cast<simd_type*>(ps))
        {
            if (ps == nullptr && s)
                throw std::invalid_argument("nullptr to construct a column");
            if (!is_aligned_simd<float_type>(s))
                throw std::invalid_argument("unaligned length is not allowed");
        }
    };
    /**
     * @brief default template implmention of column
     * 
     * @warning never use it, its designed to be specilized
     * 
     * @tparam T value type
     */
    template <typename T> class column : public basic_column<T>
    {
    };
    /**
     * @brief a column which stores a float
     * 
     * 
     * @headerfile database.hpp
     * @tparam T value type
     */
    template <> class column<float> : public basic_column<float>
    {
    public:
        virtual value_type sum() override;
        virtual value_type min() override;
        virtual value_type max() override;
    };
    class table
    {
    };
} // namespace whimap
