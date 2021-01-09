#include "libchat/client.hpp"

boost::asio::ip::tcp::endpoint ep( boost::asio::ip::address::from_string("127.0.0.1"), 8001);

boost::asio::io_service io_service;

libclient::client::client(const std::string& name):socket(io_service),_is_started(true),client_name(name)
{
    std::cout<<"Starting client "<<client_name<<std::endl;
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
   std::cout<<"Sending login request"<<std::endl;
   _write_to_socket(_LOGIN_REQUEST_+client_name+"\n");
   _read_from_socket();
   while(_is_started)
   {
       _write_to_socket(_PING_REQUEST_+"\n");
       _read_from_socket();
        process_message();
       int sl = rand()%7000;
       std::cout<<client_name<<" ping "<<sl<<std::endl;
       boost::this_thread::sleep(boost::posix_time::millisec(sl));

   }

}

void libclient::client::_read_from_socket() 
{
    _num_bytes_read = 0;
    boost::asio::read(socket, boost::asio::buffer(incomming_message), 
            boost::bind(&libclient::client::read_complete, this, boost::placeholders::_1,boost::placeholders::_2));
}

size_t libclient::client::read_complete(const boost::system::error_code & err, size_t bytes) 
{
    // Read one-by-one until we get to enter, no buffering
    
    if ( err)
    {
        std::cout<<err<<std::endl;
        return 0;

    } 
    _num_bytes_read = bytes;
    size_t pos = std::find(incomming_message, incomming_message + bytes, '\n')-incomming_message;
    bool found = pos < bytes;
    return found ? 0 : 1;
}

void libclient::client::process_message()
{
    std::string msg(incomming_message, _num_bytes_read);
    if ( msg.find(_LOGIN_SUCCESS_) == 0) _handle_login();
    else if ( msg.find(_PING_RESPONSE_) == 0) _respond_ping(msg);
    else if ( msg.find(_RESP_CLIENT_LIST_) == 0) _on_clients_change(msg);
    else std::cerr << "invalid msg " << msg << std::endl;
}

void libclient::client::_handle_login() 
{
    std::cout <<client_name<< " LOGIN SUCCESS" << std::endl;
}

void libclient::client::_request_clients()
{
    _write_to_socket(_REQ_CLIENT_LIST_+"\n");
}

void libclient::client::_write_to_socket(const std::string& _send_message)
{   
    socket.write_some(boost::asio::buffer(_send_message));
}

void libclient::client::_respond_ping(const std::string& _send_message)
{
    _request_clients();
    _read_from_socket();
}

void libclient::client::_on_clients_change(const std::string& _send_message)
{
    std::cout<<"New client list is \n"<<_send_message<<std::endl;
}

void libclient::client_thread(const std::string & client_name) 
{
    client cl(client_name);
    try 
    {
        cl.init_connection(ep);
        cl.client_loop();
    } 
    catch(boost::system::system_error & err) {
        std::cout << "client terminated " << cl.client_name 
                << ": " << err.what() << std::endl;
    }
}

void libclient::main_client_thread() {
    boost::thread_group threads;
    char* names[] = { "John", "James", "Lucy", "Tracy", "Frank", "Abby", 0 };
    for ( char ** name = names; *name; ++name) 
    {
        threads.create_thread( boost::bind(client_thread, *name));
        boost::this_thread::sleep( boost::posix_time::millisec(100));
    }
    threads.join_all();
}