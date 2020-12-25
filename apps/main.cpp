#include "libmain/chat.hpp"

using namespace std;
using namespace libmain;
// using namespace boost;

void handle_client(){
    chat cl;
    cl.get_inp_message();
    cl.send_message();
}

void display_client(){
    chat cl{"127.0.0.1",50000,"Shreyas"};
    cl.receive_message();
    cl.display_received();
}

int main() {
    // boost::thread  c{handle_client},t{display_client};
    // if(c.joinable())
    // c.join();
    // if(t.joinable())
    // t.join();
    handle_client();
    return 0;
}
