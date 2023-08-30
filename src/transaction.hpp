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
#include <future>
#include <thread>
#include <vector>
#include <functional>

#include "database.hpp"
#include "transcation_impl.hpp"
namespace whimap
{
    using worker_base = std::thread;
    class worker : public worker_base
    {
    };
    class transaction : std::future<void>
    {
        friend class worker;

    private:
    public:
        using type_enum = enum {
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
        };
        static_assert(
            impl::size_impl == TRANSACTION_MAX,
            "unmatched number of transaction type_enum to transaction impl");
        using job_type = impl::job_type;
    private:
        std::vector<transaction&> dependencies;
        job_type                  action;
        type_enum                 type;

    public:
        transaction(const transaction&) = delete;
        transaction(transaction&&);
        transaction(type_enum t);
    };
} // namespace whimap