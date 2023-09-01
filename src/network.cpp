#include "network.hpp"
#include "parser.hpp"
using namespace whimap;
server::server(int prt)
    : acceptor(context)
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
    while (true)
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
