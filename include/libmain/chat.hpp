#pragma once

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

#ifndef _MAX_BUF_SIZE_
#define _MAX_BUF_SIZE_ 1024
#endif

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> _socket_ptr;

namespace libmain {
    class chat{
        private:
            std::string input_message;
            std::string incomming_message;
            std::string client_name;
            std::string address;
            unsigned short port;
            boost::asio::io_service service;
            // _socket_ptr socket_ptr(boost::asio::ip::tcp::socket::basic_socket);
        public:
            chat();
            chat(std::string add,unsigned short p,std::string name);
            // ~chat();
            void get_inp_message();
            void send_message();
            void receive_message();
            void display_received();
    };
}
