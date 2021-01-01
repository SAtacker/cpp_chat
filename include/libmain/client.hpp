#pragma once

#include <boost/asio.hpp>
#include <string>
#include <unordered_map>
#include <iostream>

#define _HASH_PRIME_ 7
#define _LOGIN_REQ_ 'l'
#define _LOGIN_SUCCESS_ 'o'
#define _LOGIN_FAIL_ 'f'
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
            std::string unique_id_received;
            unsigned short port;
            boost::asio::io_service& _service,service;
            boost::asio::ip::tcp::socket socket;
            bool quit;
        public:
            client();
            client(std::string add,unsigned short p,std::string name);
            void get_inp_message();
            void send_message();
            void receive_message();
            void display_received();
            void login_client();
            bool is_quit();
    };
}