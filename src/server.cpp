#include "libmain/server.hpp"
#include <iostream>

using namespace std;
using namespace libmain;
server::server():_service(service),socket(service),acceptor_server(service,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)){
    cout<<"============= Welcome to cpp_chat! =====================\n";
    cout<<"Enter ip: ";
    cin>>address;
    cout<<"\nEnter endpoint port number: ";
    cin>>port;
    // TODO: Handle errors o connecting sockets
    // Listening for any new incomming connection 
    // at port 9999 with IPv4 protocol 
    // acceptor_server( 
    //     service, 
    //     boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)); 
  
    // Creating socket object 
  
    // waiting for connection 
    acceptor_server.accept(socket); 
}


server::server(std::string add,unsigned short p):_service(service),socket(service),acceptor_server(service,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), p)){
    address = add;
    port = p;
    // socket_ptr(*service);
    // https://stackoverflow.com/questions/50091228/c-error-use-of-deleted-function-boostasioio-contextio-context
    // TODO: Handle errors o connecting sockets
    // socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(address),port));
    // Listening for any new incomming connection 
    // at port 9999 with IPv4 protocol 
  
    // Creating socket object 
  
    // waiting for connection 
    acceptor_server.accept(socket); 
}

void server::receive_message(){
    // Get message from server
    boost::asio::streambuf buf;
    boost::system::error_code ec;
    boost::asio::read_until(socket,buf,"\n",&ec);
    if(ec == boost::asio::error::eof || !ec){
        //TODO: Handle error
    }
    incomming_message = boost::asio::buffer_cast< const char*>(buf.data());
}

void server::display_received(){
    cout<<incomming_message;
    cout<<endl;
}

void send_response(){
    //TODO: send response
}

void server::init_server(){

    // receive_message();
    
    while (true) { 
  
        // Fetching response 
        receive_message();
        incomming_message.pop_back();
        // Validating if the connection has to be closed 
        if (incomming_message == "exit") { 
            cout << " left!" << endl; 
            break; 
        } 
        cout<<endl;
        cout << "Received Message : ";
        display_received();
    } 
}