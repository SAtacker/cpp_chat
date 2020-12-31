#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#ifndef _MAX_BUF_SIZE_
#define _MAX_BUF_SIZE_ 1024
#endif

namespace libmain {
    class server{
        private:
            std::string incomming_message;
            std::string address;
            unsigned short port;
            boost::asio::io_service& _service,service;
            boost::asio::ip::tcp::socket socket;
            boost::asio::ip::tcp::acceptor acceptor_server;
        public:
            server();
            server(std::string add,unsigned short p);
            void init_server();
            void receive_message();
            void display_received();
    };
}
