#include "libmain/server.hpp"
#include <iostream>

using namespace std;
using namespace libmain;

std::unordered_map<std::string,size_t> libmain::server::user_names_hash;
std::unordered_map<std::string,std::string> libmain::server::hash_user_names;


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
    incomming_message.pop_back();
    time_t t = time(NULL);
    string opt = "[ ";
    opt+=ctime(&t);
    opt.pop_back();
    opt+= " ] : ";
    opt+= hash_user_names.find(incomming_message.substr(0,incomming_message.find_first_of("\n")))->second;
    opt.pop_back();
    opt+= " : ";
    cout<<opt;
    cout<<&incomming_message.at(incomming_message.find_first_of("\n")+1);
    cout<<endl;
}

void server::send_response(){
    response = response + "\n";
    boost::asio::write(socket,boost::asio::buffer(response));
}

void server::handle_login(){
    receive_message();
    if(incomming_message[0]==_LOGIN_REQ_){
        cout<<"Received login request from "<<incomming_message.substr(1)<<endl;
        if(&incomming_message.at(incomming_message.length()-1)=="\n")incomming_message.pop_back();
        if(user_names_hash.find(incomming_message.substr(1))!=user_names_hash.end() || user_names_hash.size()==0){
            hashed_id = hash<string> {} (incomming_message.substr(1));
            user_names_hash[incomming_message.substr(1)] = hashed_id;
            hash_user_names[to_string(hashed_id)] = incomming_message.substr(1);
            response = _LOGIN_SUCCESS_ + to_string(hashed_id);
            // cout<<"Sending response "<<response<<endl;
            send_response();
        }else{
            cout<<"Username taken. Please use another"<<endl;
            response = _LOGIN_FAIL_;
            send_response();
        }
    }
    else if(user_names_hash.find(hash_user_names[incomming_message.substr(0,sizeof(size_t))])==user_names_hash.end()){
        display_received();
    }
    else{
        cout<<"Error logging the user"<<"\nReceived : "<<incomming_message<<endl;
    }
}

void server::init_server(){
    acceptor_server.accept(socket);
    while (true) {
        // Fetching response 
        handle_login();
        // Validating if the connection has to be closed 
        if (incomming_message.substr(incomming_message.find_first_of("\n")+1) == "exit") { 
            cout << " left!" << endl;
            break; 
        }
    } 
}