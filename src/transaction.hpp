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
#include "transcation_impl.hpp"
namespace whimap
{
    using worker_base = std::thread;
    class worker : public worker_base
    {
    };
    template <typename T>
    class transaction : std::future<T>
    {
        friend class worker;

    public:
        using type_enum = job_enum;
        using col_type  = T;

    private:
        std::vector<transaction*> dependencies;
        col_type&                 target;
        type_enum                 type;

    public:
        transaction(type_enum t, T& col);
    };
} // namespace whimap
