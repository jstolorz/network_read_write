//
// Created by janusz on 04.02.2022.
//

#pragma once

#include <boost/asio.hpp>
#include <optional>
#include <queue>
#include <unordered_set>
#include <iostream>
#include <string>

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;
using namespace std::placeholders;

class session : public std::enable_shared_from_this<session> {
public:
    session(tcp::socket&& socket)
    : socket(std::move(socket))
    , incoming(65536)
    {
    }

    void start(){
        read();
    }

private:
    void read() {
        io::async_read(socket, incoming, io::transfer_at_least(1),
                       std::bind(&session::on_read, shared_from_this(), _1, _2));
    }

    void on_read(error_code ec, std::size_t bytes_transferred) {
        if (!ec && bytes_transferred) {
            if(!writing){
               write();
            }

            read();
        }else{
            close();
        }
    }

    void write() {

        writing = true;

        auto buffer = outgoing.prepare(incoming.size());
        io::buffer_copy(buffer, incoming.data());
        outgoing.commit(buffer.size());

        incoming.consume(incoming.size());

        io::async_write(socket, outgoing, std::bind(&session::on_write, shared_from_this(), _1, _2));
    }

    void on_write(error_code ec, std::size_t bytes_transferred) {
        writing = false;

        if(errno){
            close();
        }
    }

    void close(){
        error_code error;
        socket.close(error);
    }

private:
    tcp::socket socket;
    bool writing = false;
    io::streambuf incoming;
    io::streambuf outgoing;
};


class server {
public:

    server(io::io_context& io_context, std::uint16_t port)
    : io_context(io_context)
    , acceptor(io_context, tcp::endpoint(tcp::v4(),port))
    {
        accept();
    }

private:
    void accept() {
       socket.emplace(io_context);

       acceptor.async_accept(*socket,[&](error_code){
           std::make_shared<session>(std::move(*socket))->start();
       });
    }

private:
    io::io_context &io_context;
    tcp::acceptor acceptor;
    std::optional<tcp::socket> socket;
};

int start_server() {

    io::io_context io_context;
    server srv(io_context, 9999);
    io_context.run();


    return 0;
}