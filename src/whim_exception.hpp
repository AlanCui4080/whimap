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
#include <stdexcept>
#include <exception>
#include <type_traits>
#include <system_error>

#include <cstring>
namespace whimap
{
    class network_failure : public std::system_error
    {
        public:
        network_failure(const std::remove_reference_t<decltype(errno)> v,const char* s)
        : system_error(std::error_code(v,std::generic_category()),s){};
    };
    class simd_failure : public std::domain_error
    {
        public:
        simd_failure(const char* s)
        : domain_error(s){};
    };
} // namespace whimap
