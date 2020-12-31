#include "libmain/client.hpp"
#include "libmain/server.hpp"

using namespace std;
using namespace libmain;

void handle_client(){
    client cl{"127.0.0.1",5100,"Human"};
    while(true){
        cl.get_inp_message();
        cl.send_message();
    }
}

void serrver_echo(){
    server s{"127.0.0.1",5100};
    s.init_server();
}

int main() {
    boost::thread c{handle_client},t{serrver_echo};
    if(c.joinable())
    c.join();
    if(t.joinable())
    t.join();
    return 0;
}
