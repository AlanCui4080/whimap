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
#include <iostream>
#include <numeric>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "whim_exception.hpp"
namespace whimap
{
    class socketbuf : public std::streambuf
    {
    public:
        using socket_descriptor = int;

    private:
        socket_descriptor socket_fd;

    public:
        virtual auto xsgetn(char_type* s, std::streamsize n)
            -> std::streamsize override;
        virtual auto xsputn(const char_type* s, std::streamsize n)
            -> std::streamsize override;

    private:
        auto fail_on_error() -> void
        {
            int       error = 0;
            socklen_t len   = sizeof(error);
            auto      result =
                getsockopt(socket_fd, SOL_SOCKET, SO_ERROR, &error, &len);
            if (result < 0)
                throw network_failure(result, "failed to getsockopt");
            if (error < 0)
                throw network_failure(
                    error, "a socket in error status is not acceptable");
        }

    public:
        socketbuf(const socketbuf&);
        socketbuf(socketbuf&&);
        socketbuf(socket_descriptor&& fd);
    };
    class socketstream : public std::iostream
    {
    public:
        using socket_descriptor = socketbuf::socket_descriptor;

    private:
        socketbuf buffer;

    public:
        socketstream(socket_descriptor&& fd)
            : buffer(std::forward<socket_descriptor>(fd))
            , std::iostream(&buffer){};
        socketstream(const socketstream& old)
            : buffer(old.buffer){};
        socketstream(socketstream&& old)
            : buffer(std::forward<socketbuf>(old.buffer)){};
    };
} // namespace whimap
