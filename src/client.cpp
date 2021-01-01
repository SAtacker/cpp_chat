#include "libmain/client.hpp"

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
    login_client();
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
    login_client();
}

void client::login_client(){
    input_message = _LOGIN_REQ_ + client_name+ "\n";
    boost::asio::write(socket,boost::asio::buffer(input_message));
    receive_message();
    if(incomming_message[0]==_LOGIN_SUCCESS_){
        cout<<"Login Success"<<endl;
        unique_id_received.clear();
        unique_id_received.resize(incomming_message.length()-1);
        if(&incomming_message.at(incomming_message.length()-1)=="\n")incomming_message.pop_back();
        for(int i{0};i<incomming_message.length()-1;i++){
            unique_id_received[i] = incomming_message[i+1]; 
        }
        incomming_message.clear();
        quit = false;
    }
    else if(incomming_message[0]==_LOGIN_FAIL_){
        cout<<"Login failed. Try new username"<<endl;
        quit = false;
    }
    else{
        cout<<"Login Error"<<endl;
        quit = true;
    }

}

void client::get_inp_message(){
    cout<<endl;
    cout<<client_name<<" : ";
    getline(cin,input_message);
}

void client::receive_message(){
    // Get message from server
    boost::asio::streambuf buf;
    boost::asio::read_until(socket,buf,"\n");
    incomming_message = boost::asio::buffer_cast< const char*>(buf.data());
}

void client::send_message(){
    // Write message to the socket
    string temp = input_message;
    input_message = unique_id_received + input_message + "\n";
    boost::asio::write(socket,boost::asio::buffer(input_message));
    if(temp=="exit"){
        cout<<"Exiting"<<endl;
        socket.close();
        quit=true;
    }
}

void client::display_received(){
    cout<<incomming_message;
}

bool client::is_quit(){
    return quit;
}