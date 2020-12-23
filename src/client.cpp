#include "libmain/client.hpp"
#include <iostream>

using namespace std;
using namespace libmain;

client::client(){
    cout<<"============= Welcome to cpp_chat! =====================\n";
    cout<<"Enter ip: ";
    cin>>address;
    cout<<"\nEnter a port number: ";
    cin>>port;
}

void client::get_inp_message(){
    // Get message from input buffer
    boost::asio::streambuf buf;
    cout<<"Enter Message: ";
    boost::asio::read_until(socket,buf,"\n");
    input_message = boost::asio::buffer_cast< const char*>(buf.data());

}

void client::send_message(){
    // Write message to the socket
    boost::asio::write(socket,boost::asio::buffer(input_message));
}