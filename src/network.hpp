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
#include <vector>
#include <asio.hpp>
#include <mutex>
#include <future>
#include <semaphore>
#include "whim_exception.hpp"
#include "transaction.hpp"
namespace whimap
{
    class session
    {
    private:
        asio::ip::tcp::socket peer;

    public:
        session() = delete;
        session(asio::ip::tcp::socket&& pr);
    };
    class server
    {
    private:
    public:
        static constexpr auto port = 7878;

    private:
        asio::io_context        context;
        asio::ip::tcp::acceptor acceptor;
        asio::ip::tcp::endpoint endpoint;
        std::vector<session>    session_list;

    public:
        server(int prt = port, /*FOR TEST: MUST AT END**/ bool do_no_exit = true);
        //for test
        explicit server(nullptr_t)
            : server(port, false){};
    };
} // namespace whimap
