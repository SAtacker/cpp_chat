#pragma once

#include <boost/asio.hpp>
#include <unordered_map>
#include <time.h>

#define _HASH_PRIME_ 7
#define _LOGIN_REQ_ 'l'
#define _LOGIN_SUCCESS_ 'o'
#define _LOGIN_FAIL_ 'f'
#ifndef _MAX_BUF_SIZE_
#define _MAX_BUF_SIZE_ 1024
#endif

namespace libmain {
    class server{
        private:
            std::string incomming_message;
            std::string address;
            std::string response;
            static std::unordered_map<std::string,size_t> user_names_hash;
            static std::unordered_map<std::string,std::string> hash_user_names;
            unsigned short port;
            size_t hashed_id;
            boost::asio::io_service& _service,service;
            boost::asio::ip::tcp::socket socket;
            boost::asio::ip::tcp::acceptor acceptor_server;
        public:
            server();
            server(std::string add,unsigned short p);
            void init_server();
            void send_response();
            void handle_login();
            void receive_message();
            void display_received();
    };
}