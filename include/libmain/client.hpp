#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#ifndef _MAX_BUF_SIZE_
#define _MAX_BUF_SIZE_ 1024
#endif

namespace libmain {
    class client{
        private:
            std::string input_message;
            std::string incomming_message;
            std::string client_name;
            std::string address;
            unsigned short port;
            boost::asio::io_service& _service,service;
            boost::asio::ip::tcp::socket socket;
        public:
            client();
            client(std::string add,unsigned short p,std::string name);
            void get_inp_message();
            void send_message();
            void receive_message();
            void display_received();
    };
}
