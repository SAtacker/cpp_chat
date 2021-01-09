#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>

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


namespace libserver{
    class server: public boost::enable_shared_from_this<server>
    {
    private:
        static std::vector<std::string> cl_list;
        boost::asio::ip::tcp::socket socket;
        int _num_bytes_read;
        char _incomming_message[_MAX_BUF_SIZE_];
        bool _is_reading;
        bool _map_change;
        std::string client_name;
        boost::posix_time::ptime _ping;
        void _read_from_socket();
        void _process_request_();
        void _handle_login_req();
        void _handle_ping_req();
        void _handle_list_request();
        void _write_to_socket(const std::string& _send_message);
        void _display_message(const std::string& _display_msg);
    public:
        server();
        void handle_client();
        void _set_map_change();
        bool _is_timed_out() const;
        void _exit();
        const std::string _get_username();
        boost::asio::ip::tcp::socket& _get_socket();
    };
    void _update_client_changed();
    void multiple_client_handler();
    void acceptor_thread();
}
