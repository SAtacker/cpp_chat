#include "libchat/chat.hpp"

libclient::client::client(const std::string& name):socket(io_service),_is_started(false),client_name(name)
{
    std::cout<<"Starting client"<<std::endl;
}

void libclient::client::init_connection(boost::asio::ip::tcp::endpoint ep)
{
    // Connect to the specified endpoint
    socket.connect(ep);
}

void libclient::client::client_loop()
{
    /*
        Check if the client has received login success from server and proceed
    */

}