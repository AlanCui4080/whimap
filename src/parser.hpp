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
#include <sstream>
#include <asio.hpp>
#include <cstdio>
#include <vector>
#include "database.hpp"
#include "transaction.hpp"
namespace whimap
{
    class parser
    {
    public:
        using file_descriptor = asio::detail::socket_type;
        using file_pointer    = std::FILE*;

        enum sql
        {
            SELECT,
            WHERE,
            FROM,
            AND,
            OR,
            CREATE,
            DROP,
            TABLE,
        };
        std::vector<sql> lex;

    public:
        parser(file_pointer ifp);
        parser(file_descriptor ifd);
        auto lexdata() const -> const std::vector<sql>&;
    };
} // namespace whimap
