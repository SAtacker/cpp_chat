#include "libmain/hello_world.hpp"

std::string libmain::helloWorld() {
    return std::string{"Hello world!"};
}

std::string libmain::get_message(boost::asio::ip::tcp::socket& socket ){
    // Gets an input message to be delivered

    boost::asio::streambuf input_buff;
    boost::asio::read_until(socket,input_buff,"\n");
    std::string data = boost::asio::buffer_cast< const char* >(input_buff.data());
    return data;
}

void libmain::send_message(boost::asio::ip::tcp::socket& socket , std::string& message){
    boost::asio::write(socket,boost::asio::buffer(message+"\n"));
}

