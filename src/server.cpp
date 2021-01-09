#include "libchat/server.hpp"


boost::asio::io_service io_service;
typedef boost::shared_ptr<libserver::server> server_ptr;
typedef std::vector<server_ptr> server_ptr_vector;
server_ptr_vector server_shared_ptrs;
// thread-safe access to clients array
boost::recursive_mutex cs;

/*
    Regarding shared from this https://stackoverflow.com/questions/3629557/boost-shared-from-this

    When a single connection ends, all associated asynchronous operations complete. 
    The corresponding handler objects are destroyed, and all shared_ptr references to the objects are destroyed.
    The io_service destructor destroys all handlers, causing all shared_ptr references to all connection objects to be destroyed.

*/


libserver::server::server() : socket(io_service)
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

bool libserver::server::_is_timed_out() const
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

        // Store the message in a std::string container
        std::string msg(_incomming_message,(std::find(_incomming_message,_incomming_message+_num_bytes_read,'\n')-_incomming_message));

        // Copy the read message into buffer (_incomming_message)
        std::copy(_incomming_message+_num_bytes_read,_incomming_message+_MAX_BUF_SIZE_,_incomming_message);

        _num_bytes_read -= (std::find(_incomming_message,_incomming_message+_num_bytes_read,'\n')-_incomming_message);
        if(msg.find(_LOGIN_REQUEST_)==0)
        {   
            client_name = msg.substr((_LOGIN_REQUEST_).length());
            _handle_login_req();
            libserver::_update_client_changed();
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
            std::string m;
            m = msg.substr((_REQ_ECHO_).length());
            _display_message(m);
        }
    }
}

void libserver::server::_handle_login_req()
{
    // TODO::Handle usning unordered map
    std::cout<<client_name<<" logged in"<<std::endl;
    _write_to_socket(_LOGIN_SUCCESS_+"\n");
    _set_map_change();
}

void libserver::server::_handle_ping_req()
{
    // Respond to ping request
    std::cout<<"Received ping from "<<client_name<<std::endl;
    std::cout<<"Last ping: "<<boost::posix_time::to_simple_string(_ping)<<std::endl;
    if(_map_change)
    {
        _handle_list_request();
    }
    else
    {
        _write_to_socket(_PING_RESPONSE_+"\n");
    }
    _ping = boost::posix_time::microsec_clock::local_time();
    _map_change = false;
}

void libserver::server::_handle_list_request()
{
    // Respond with Usernames of people currently
    std::string unames;
    {
        boost::recursive_mutex::scoped_lock lk(cs);
        for(auto it:server_shared_ptrs)
        {
            unames+=(it)->_get_username();
        }
        unames+="\n";
    }
    unames = _RESP_CLIENT_LIST_ + unames;
    _write_to_socket(unames);

}

void libserver::server::_display_message(const std::string& _display_msg)
{
    // Display a nice formatted message
    std::cout<<client_name<<" : "<<_display_msg<<std::endl;
}


void libserver::server::_write_to_socket(const std::string& _send_message)
{   
    boost::system::error_code ec;
    boost::asio::write(socket,boost::asio::buffer(_send_message),boost::asio::transfer_exactly(_send_message.length()),&ec);

    if(ec)
    {
        std::cout<<"Error while writing "<<ec<<std::endl;
    }

}

const std::string libserver::server::_get_username()
{
    return client_name;
}

boost::asio::ip::tcp::socket& libserver::server::_get_socket() 
{
    return socket; 
}

void libserver::_update_client_changed()
{
    boost::recursive_mutex::scoped_lock lk(cs);
    for(auto b:server_shared_ptrs)
    {
        (b)->_set_map_change();
    }
}

void libserver::multiple_client_handler()
{
    while ( true) {
        boost::this_thread::sleep( boost::posix_time::millisec(1));
        for (auto b:server_shared_ptrs) 
            (b)->handle_client();
        // erase clients that timed out
        boost::recursive_mutex::scoped_lock lk(cs);
        server_shared_ptrs.erase(std::remove_if(server_shared_ptrs.begin(), server_shared_ptrs.end(), 
                   boost::bind(&libserver::server::_is_timed_out,boost::placeholders::_1)), server_shared_ptrs.end());
    }
}

void libserver::acceptor_thread()
{
    boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8001));
    while ( true) 
    {
        server_ptr _new_(new libserver::server);
        acceptor.accept(_new_->_get_socket());
        boost::recursive_mutex::scoped_lock lk(cs);
        server_shared_ptrs.push_back(_new_);
    }
}
