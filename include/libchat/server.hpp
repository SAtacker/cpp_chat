#include <boost/asio.hpp>
#include <boost/thread.hpp>

#define _MAX_BUF_SIZE_ 1024
#define _LOGIN_SUCCESS_ 'ok'
#define _LOGIN_REQUEST_ 'lo'
#define _PING_REQUEST_ 'ping'
#define _REQ_CLIENT_LIST_ 'list'
#define _REQ_ECHO_ 'echo'
#define _MAX_PING_DELAY_MSEC_ 5000

namespace libserver{
    class server
    {
    private:
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
        void _display_message();
    public:
        server(boost::asio::io_service service);
        ~server();
        void handle_client();
        void _set_map_change();
        bool _is_timed_out();
        void _exit();
    };
}