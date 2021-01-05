#include "libchat/server.hpp"

libserver::server::server(boost::asio::io_service service) : socket(service)
{
    _is_reading = false;
    _num_bytes_read = 0;
    _ping = boost::posix_time::microsec_clock::local_time();
}

void libserver::server::handle_client()
{
    /* 
        Main sequential client handler
        Reads from socket
        Processes the message read
    */
    // TODO: Handle errors in reading
    _read_from_socket();
    _process_request_();
    if (_is_timed_out())
    {
        _exit();
    }
}

void libserver::server::_set_map_change()
{
    _map_change = true;
}

bool libserver::server::_is_timed_out()
{
    return ((boost::posix_time::microsec_clock::local_time() - _ping).total_milliseconds()) > _MAX_PING_DELAY_MSEC_;
}

void libserver::server::_exit()
{
    /* 
        Closes the current client connection 
    */
    // TODO: Handle errors in closing socket
    socket.close();
}

void libserver::server::_read_from_socket()
{
    if (socket.available())
    {
        _num_bytes_read += socket.read_some(boost::asio::buffer(_incomming_message + _num_bytes_read, (_MAX_BUF_SIZE_ - _num_bytes_read)));
    }
}

void libserver::server::_process_request_()
{
    /*
        Check if complete message is read till newline character
        Handle the corresponding request
    */
    if(std::find(_incomming_message,_incomming_message+_num_bytes_read,'\n'))
    {
        // Update ping
        _ping = boost::posix_time::microsec_clock::local_time();

        // Store the message in a std::string container
        std::string msg(_incomming_message,(std::find(_incomming_message,_incomming_message+_num_bytes_read,'\n')-_incomming_message));

        // Copy the read message into buffer (_incomming_message)
        std::copy(_incomming_message+_num_bytes_read,_incomming_message+_MAX_BUF_SIZE_,_incomming_message);

        _num_bytes_read -= (std::find(_incomming_message,_incomming_message+_num_bytes_read,'\n')-_incomming_message) + 1;

        if(msg.find(_LOGIN_REQUEST_)==0)
        {
            _handle_login_req();
        }
        else if(msg.find(_PING_REQUEST_)==0)
        {
            _handle_ping_req();
        }
        else if(msg.find(_REQ_CLIENT_LIST_) == 0)
        {
            _handle_list_request();
        }
        else if(msg.find(_REQ_ECHO_)==0)
        {
            _display_message();
        }
    }
}

void libserver::server::_handle_login_req()
{
    // Handle usning unordered map
}

void libserver::server::_handle_ping_req()
{
    // Respond to ping request
}

void libserver::server::_handle_list_request()
{
    // Respond with Usernames of people currently
}

void libserver::server::_display_message()
{
    // Display a nice formatted message
}


void libserver::server::_write_to_socket(const std::string& _send_message)
{
    socket.write_some(boost::asio::buffer(_send_message));
}