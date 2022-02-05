//
// Created by janusz on 03.02.2022.
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum {
    max_length = 1024
};

int main() {

    try {

        boost::asio::io_context io_context;

        tcp::socket s(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(s, resolver.resolve("127.0.0.1", "9999"));

        char outgoing[] = "Some outgoing message\n";
        char incoming[256];
        boost::asio::streambuf streambuf;

        s.async_send(boost::asio::buffer(outgoing), [&]
        (boost::system::error_code ec, std::size_t length){
            std::cout << ec.message() << ", bytes transferred: " << length << "\n";
        });


        boost::asio::async_read(s,streambuf,[&]
        (boost::system::error_code ec, std::size_t length){
            std::cout << "Data: " << std::istream(&streambuf).rdbuf() << "\n";
            streambuf.consume(length);
        });

//        s.async_receive(boost::asio::buffer(incoming),[&]
//        (boost::system::error_code ec, std::size_t length){
//            std::cout << ec.message() << ", bytes received: " << length
//            << " data incoming: " << incoming << "\n";
//        });

        io_context.run();

//    std::cout << "Enter message: ";
//    char request[max_length];
//
//    std::cin.getline(request, max_length);
//    size_t request_length = std::strlen(request);
//    boost::asio::write(s, boost::asio::buffer(request, request_length));

//    char reply[max_length];
//    size_t reply_length = boost::asio::read(s,
//        boost::asio::buffer(reply, request_length));
//    std::cout << "Reply is: ";
//    std::cout.write(reply, reply_length);
//    std::cout << "\n";
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }


    return 0;
}