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
#include <hwy/aligned_allocator.h>
#include <hwy/highway.h>
namespace whimap
{
    namespace simd
    {
        using namespace hwy;
        using namespace hwy::HWY_NAMESPACE;
    };
    /**
     * @brief a psedo, continous vector of type S
     * 
     * @tparam S scalar type
     */
    template <typename S> class expvec
    {
    public:
        using scalar_type           = S;
        using simd_type             = simd::ScalableTag<scalar_type>;
        using vector_type           = simd::VFromD<simd_type>;
        using scalar_unique_ptr     = simd::AlignedFreeUniquePtr<scalar_type[]>;
        using vector_unique_ptr     = simd::AlignedFreeUniquePtr<vector_type[]>;
        static constexpr auto D_tag = simd_type();

    private:
        vector_unique_ptr vector_data;
        size_t            vector_count;

    public:
        expvec(scalar_unique_ptr&& pa, size_t count)
            : vector_data(reinterpret_cast<vector_type*>(pa.release()))
            , vector_count(count / simd::Lanes(D_tag))
        {
        }
        constexpr auto size() const
        {
            return vector_count;
        }
        constexpr auto operator[](const size_t i) const -> const vector_type&
        {
            return vector_data[i];
        }
        constexpr auto operator[](const size_t i) -> vector_type&
        {
            return vector_data[i];
        }

    private:
        using op2_type = decltype(simd::Add<vector_type>);
        template <op2_type Opa>
        inline auto op2(const expvec<S>& rv) const -> expvec<S>
        {
            if (rv.size() != vector_count)
                throw simd_failure(
                    "tring to operate between two different sized vector");
            auto result = expvec(simd::AllocateAligned<scalar_type>(
                                     vector_count * simd::Lanes(D_tag)),
                                        vector_count * simd::Lanes(D_tag));
            for (size_t i = 0; i < vector_count; i++)
            {
                result[i] = std::move(Opa(vector_data[i], rv.vector_data[i]));
            }
            return result;
        }

    public:
        auto operator+(const expvec<S>& rv) const -> expvec<S>
        {
            return std::move(op2<simd::Add>(rv));
        }
        auto operator-(const expvec<S>& rv) const -> expvec<S>
        {
            return std::move(op2<simd::Sub>(rv));
        }
        auto operator*(const expvec<S>& rv) const -> expvec<S>
        {
            return std::move(op2<simd::Mul>(rv));
        }
        auto operator/(const expvec<S>& rv) const -> expvec<S>
        {
            return std::move(op2<simd::Div>(rv));
        }
        auto operator|(const expvec<S>& rv) const -> expvec<S>
        {
            return std::move(op2<simd::Or>(rv));
        }
        auto operator&(const expvec<S>& rv) const -> expvec<S>
        {
            return std::move(op2<simd::And>(rv));
        }
        auto operator<<(const expvec<S>& rv) const -> expvec<S>
        {
            return std::move(op2<simd::ShiftLeft>(rv));
        }
        auto operator>>(const expvec<S>& rv) const -> expvec<S>
        {
            return std::move(op2<simd::ShiftRight>(rv));
        }
    };
} // namespace whimap
