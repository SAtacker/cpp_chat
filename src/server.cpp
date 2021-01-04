#include "libmain/server.hpp"
#include <iostream>

using namespace std;
using namespace libmain;

std::unordered_map<std::string,size_t> libmain::server::user_names_hash;
std::unordered_map<std::string,std::string> libmain::server::hash_user_names;


server::server():_service(service),socket(service),secondary_socket(_service),acceptor_server(service,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)){
    cout<<"============= Welcome to cpp_chat! =====================\n";
    cout<<"Enter ip: ";
    cin>>address;
    cout<<"\nEnter endpoint port number: ";
    cin>>port; 
}


server::server(std::string add,unsigned short p):_service(service),secondary_socket(_service),socket(service),acceptor_server(service,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), p)){
    address = add;
    port = p;

}

void server::receive_message(){
    // Get message from server
    acceptor_server.accept(secondary_socket);
    while (true)
    {
        boost::asio::streambuf buf;
        boost::system::error_code ec;
        boost::asio::read_until(secondary_socket,buf,"\n",&ec);
        if(ec == boost::asio::error::eof || !ec){
            //TODO: Handle error
        }
        incomming_message = boost::asio::buffer_cast< const char*>(buf.data());
        cout<<incomming_message;
    }
    secondary_socket.release();
    
}

void server::display_received(){
    incomming_message.pop_back();
    time_t t = time(NULL);
    string opt = "[ ";
    opt+=ctime(&t);
    opt.pop_back();
    opt+= " ] : ";
    opt+= hash_user_names.find(incomming_message.substr(0,incomming_message.find_first_of("\n")))->second;
    opt+= " : ";
    cout<<opt;
    cout<<&incomming_message.at(incomming_message.find_first_of("\n")+1);
    cout<<endl;
}

void server::send_response(){
    response = response + "\n";
    if(socket.available())
    boost::asio::write(socket,boost::asio::buffer(response));
}

void server::handle_login(){
    // receive_message();
    acceptor_server.accept(socket);
    while(true){
        cout<<incomming_message;
        if(incomming_message[0]==_LOGIN_REQ_){
            cout<<"Received login request from "<<incomming_message.substr(1)<<endl;
            if(incomming_message.at(incomming_message.length()-1)=='\n')incomming_message.pop_back();
            if(user_names_hash.find(incomming_message.substr(1))==user_names_hash.end() || user_names_hash.size()==0){
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
        else if(user_names_hash.find(hash_user_names[incomming_message.substr(0,incomming_message.find_first_of("\n"))])!=user_names_hash.end()){
            display_received();
        }
        else if (incomming_message.substr(incomming_message.find_first_of("\n")+1) == "exit") { 
            cout << " left!" << endl;
            break; 
        }
        else{
            cout<<"Error logging the user"<<"\nReceived : "<<incomming_message<<endl;
        }
    }
    socket.release();
}

void server::init_server(){
    // acceptor_server.accept(socket);
    // while (true) {
    //     // Fetching response 
    //     handle_login();
    //     // Validating if the connection has to be closed 
    //     if (incomming_message.substr(incomming_message.find_first_of("\n")+1) == "exit") { 
    //         cout << " left!" << endl;
    //         break; 
    //     }
    // }
    boost::thread_group g;
    g.create_thread([this]{this->receive_message();});
    g.create_thread([this]{this->handle_login();});
    g.join_all(); 
}