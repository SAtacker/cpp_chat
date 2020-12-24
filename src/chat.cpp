#include "libmain/chat.hpp"
#include <iostream>

using namespace std;
using namespace libmain;

chat::chat(){
    cout<<"============= Welcome to cpp_chat! =====================\n";
    cout<<"Enter ip: ";
    cin>>address;
    cout<<"\nEnter endpoint port number: ";
    cin>>port;
    cout<<"Enter Name: ";
    cin>>client_name;
    // client_socket(io_service);
    client_socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(address),port));
    // TODO: Handle errors o connecting sockets
}

void chat::get_inp_message(){
    cout<<client_name<<" : ";
    cin>>input_message;
    input_message = client_name + " : " + input_message;
}

void chat::receive_message(){
    // Get message from server
    boost::asio::streambuf buf;
    boost::asio::read_until(socket,buf,"\n");
    incomming_message = boost::asio::buffer_cast< const char*>(buf.data());
}

void chat::send_message(){
    // Write message to the socket
    boost::asio::write(socket,boost::asio::buffer(input_message));
}

void chat::display_received(){
    cout<<incomming_message;
}