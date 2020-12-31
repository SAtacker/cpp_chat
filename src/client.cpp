#include "libmain/client.hpp"
#include <iostream>

using namespace std;
using namespace libmain;
client::client():_service(service),socket(service){
    cout<<"============= Welcome to cpp_chat! =====================\n";
    cout<<"Enter ip: ";
    cin>>address;
    cout<<"\nEnter endpoint port number: ";
    cin>>port;
    cout<<"Enter Name: ";
    cin>>client_name;
    // socket_ptr(std::ref(*service));
    // https://stackoverflow.com/questions/50091228/c-error-use-of-deleted-function-boostasioio-contextio-context
    boost::system::error_code ec;
    socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(address),port),ec);
    if (ec)
    {
        cout<<"Error occured\n";
    }
    // TODO: Handle errors o connecting sockets
}

client::client(std::string add,unsigned short p,std::string name):_service(service),socket(service){
    address = add;
    port = p;
    client_name = name;
    // socket_ptr(*service);
    // https://stackoverflow.com/questions/50091228/c-error-use-of-deleted-function-boostasioio-contextio-context
    // TODO: Handle errors o connecting sockets
    // socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(address),port));
    boost::system::error_code ec;
    socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(address),port),ec);
    if (ec)
    {
        cout<<"Error occured\n";
    }
}

void client::get_inp_message(){
    cout<<client_name<<" : ";
    cin>>input_message;
    cout<<endl;
}

void client::receive_message(){
    // Get message from server
    boost::asio::streambuf buf;
    boost::asio::read_until(socket,buf,"\n");
    incomming_message = boost::asio::buffer_cast< const char*>(buf.data());
}

void client::send_message(){
    // Write message to the socket
    boost::asio::write(socket,boost::asio::buffer(input_message+"\n"));
}

void client::display_received(){
    cout<<incomming_message;
}