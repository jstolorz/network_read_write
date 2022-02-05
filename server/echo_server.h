//
// Created by janusz on 03.02.2022.
//

#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session> {
public:

    session(tcp::socket socket)
            : socket_(std::move(socket)) {
    }

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                [this, self](boost::system::error_code ec, std::size_t length) {
                                    if (!ec) {
                                        std::cout << data_ << "\n";
                                        do_write(length);
                                    }
                                });
    }

    void do_write(std::size_t length) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_,boost::asio::buffer(data_,length),
                                 [this,self]
                                 (boost::system::error_code ec, std::size_t){
            if(!ec){
                do_read();
            }
        });
    }

private:
    tcp::socket socket_;
    enum {
        max_length = 1024
    };
    char data_[max_length];
};

class server {
public:
    server(boost::asio::io_context& context, short port)
    : acceptor_(context, tcp::endpoint(tcp::v4(),port))
    {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
                [this](boost::system::error_code ec, tcp::socket socket) {
                    if (!ec) {
                        std::make_shared<session>(std::move(socket))->start();
                    }

                    do_accept();
                });
    }

private:
    tcp::acceptor acceptor_;
};