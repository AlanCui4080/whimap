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
#include <iostream>
#include <stdexcept>
#include <exception>
#include <fstream>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "network.hpp"
#include "whim_exception.hpp"

using namespace whimap;
socketbuf::socketbuf(const socketbuf& old)
{
    socket_fd = dup(old.socket_fd);
}
socketbuf::socketbuf(socketbuf&& old)
{
    socket_fd = old.socket_fd;
}
socketbuf::socketbuf(socket_descriptor&& fd)
{
    fail_on_error();
    socket_fd = fd;
}
auto socketbuf::xsgetn(char_type* s, std::streamsize n) -> std::streamsize
{
    auto result = recv(socket_fd, s, n, 0);
    if(result < 0)
        throw network_failure(errno, "failed to recv");
}
auto socketbuf::xsputn(const char_type* s, std::streamsize n) -> std::streamsize
{
    auto result = send(socket_fd, s, n, 0);
    if(result < 0)
        throw network_failure(errno, "failed to send");
}
