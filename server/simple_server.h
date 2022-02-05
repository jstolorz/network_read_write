//
// Created by janusz on 03.02.2022.
//

#pragma once

#include <boost/asio.hpp>
#include <optional>
#include <iostream>

namespace io = boost::asio;
namespace ip = io::ip;
using tcp = ip::tcp;
using error_code = boost::system::error_code;
using namespace std::placeholders;

class session: public std::enable_shared_from_this<session>{
public:
    session(boost::asio::ip::tcp::socket&& socket)
    : socket_(std::move(socket))
    {
    }

    void start(){
        boost::asio::async_read_until(socket_,streambuf_,'\n',
                                      [self = shared_from_this()]
                                      (boost::system::error_code error, std::size_t bytes_transferred){
                std::cout << std::istream(&self->streambuf_).rdbuf();

        });
    }

private:
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf streambuf_;
};

class server{
public:
    server(boost::asio::io_context& io_context, std::uint16_t port)
    : io_context_(io_context)
    , acceptor_ (io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
    }

    void async_accept(){
        socket_.emplace(io_context_);

        acceptor_.async_accept(*socket_,[&](boost::system::error_code error){
            std::make_shared<session>(std::move(*socket_))->start();
            async_accept();
        });
    }

private:
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::optional<boost::asio::ip::tcp::socket> socket_;
};