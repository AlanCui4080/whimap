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
#include "network.hpp"
#include "parser.hpp"
using namespace whimap;
server::server(int prt, bool do_no_exit)
    : acceptor(context, asio::ip::tcp::v6())
    , endpoint(asio::ip::tcp::v6(), prt)
{
    acceptor.bind(endpoint);
    acceptor.listen();
    acceptor.async_accept(
        [&](const asio::error_code& error, asio::ip::tcp::socket peer)
        {
            if (error)
            {
                throw network_failure(error.value(), error.message().c_str());
            }
            session_list.emplace_back(session(std::move(peer)));
        });
    while (do_no_exit)
    {
        
    };
}
session::session(asio::ip::tcp::socket&& pr)
    : peer(std::move(pr))
{
    while (true)
    {
        asio::mutable_buffer buf;
        peer.receive(buf);
        auto par = parser(
            std::stringstream(std::string(static_cast<char*>(buf.data()))));
    }
}
