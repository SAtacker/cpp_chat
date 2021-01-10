#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <future>

#define _MAX_BUF_SIZE_ 1024
#define _LOGIN_SUCCESS_ (std::string)"ok"
#define _LOGIN_REQUEST_ (std::string)"lo"
#define _PING_REQUEST_ (std::string)"ping"
#define _PING_RESPONSE_ (std::string)"pok"
#define _PING_RESP_LIST_CHNG_ (std::string)"plc"
#define _REQ_CLIENT_LIST_ (std::string)"list"
#define _RESP_CLIENT_LIST_ (std::string)"clients "
#define _REQ_ECHO_ (std::string)"echo"
#define _MAX_PING_DELAY_MSEC_ 5000

namespace libclient{
    class client
    {
        private:
            boost::asio::ip::tcp::socket socket;
            int _num_bytes_read;
            char incomming_message[_MAX_BUF_SIZE_];
            bool _is_started;
            void _write_to_socket(const std::string& _send_message);
            void _read_from_socket();
            void _handle_login();
            void _request_clients();
            void _respond_ping(const std::string& _send_message);
            void _on_clients_change(const std::string& _send_message);
        public:
            std::string client_name;
            client(const std::string& name);
            void init_connection(boost::asio::ip::tcp::endpoint ep);
            void client_loop();
            size_t read_complete(const boost::system::error_code & err, size_t bytes);
            void process_message();
    };
    
    
    void client_thread(const std::string & client_name) ;

    void main_client_thread();

    static std::string getAnswer();

}