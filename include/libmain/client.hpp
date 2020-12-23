#pragma once

#include <boost/asio.hpp>

namespace libmain {
    class client{
        private:
            std::string input_message;
            std::string incomming_message;
            std::string client_name;
            std::string address;
            unsigned short port;
            boost::asio::io_service io_service;
            boost::asio::ip::tcp::socket client_socket;

        public:
            client();
            ~client();
            void get_inp_message();
            void send_message();
            void receive_message();
    };
}
