#include "libchat/server.hpp"

int main() 
{   
    boost::thread_group g;
    g.create_thread(libserver::acceptor_thread);
    g.create_thread(libserver::multiple_client_handler);
    g.join_all();
    return 0;
}
