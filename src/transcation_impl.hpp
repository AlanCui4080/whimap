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
#include <functional>

#include "database.hpp"
#include "whim_framework.hpp"
namespace whimap
{
    namespace impl
    {
        using job_type = std::function<column::float_type(const column&)>;
        static const job_type default_job =
            [](const column&) -> column::float_type {
            throw std::invalid_argument("bug: a undefined job has been called");
        };
        typedef enum
        {
            TRANSACTION_UNKNOWN,
            TRANSACTION_NUM_SUM,
            TRANSACTION_NUM_AVG,
            TRANSACTION_NUM_MIN,
            TRANSACTION_NUM_MAX,
            TRANSACTION_NUM_ROUND,
            TRANSACTION_LOGIC_COUNT,
            TRANSACTION_LOGIC_HAVING,
            TRANSACTION_LOGIC_EXISTS,
            TRANSACTION_LOGIC_GROUPBY,
            TRANSACTION_MAX,
        } type_enum;
        static const job_type jobs[] = {
            default_job,
            default_job,
            default_job,
            [](const column& v) -> column::float_type { return v.min(); },
            [](const column& v) -> column::float_type { return v.max(); },
            default_job,
            default_job,
            default_job,
            default_job,
            default_job
        };
        static constexpr auto size_impl = sizeof(jobs) / sizeof(job_type);
        static_assert(
            impl::size_impl == TRANSACTION_MAX,
            "unmatched number of transaction type_enum to transaction impl");
    } // namespace impl

} // namespace whimap
