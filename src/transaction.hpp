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
#include <any>
#include <functional>

#include "database.hpp"
namespace whimap
{
    class worker : public std::thread
    {
    };
    class transaction_base
    {
    };
    template <typename T>
    class transaction : public std::future<T>, public transaction_base
    {
        friend class worker;

    public:
        typedef enum
        {
            TRANSACTION_UNKNOWN = 0,
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
        using col_type = T;

    private:
        std::vector<transaction*> dependencies;
        col_type&                 target;
        type_enum                 type;

    public:
        transaction(type_enum t, T& col);
    };
} // namespace whimap
