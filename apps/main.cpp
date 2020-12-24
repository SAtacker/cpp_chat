#include "libmain/chat.hpp"
#include <boost/thread.hpp>

using namespace std;
using namespace libmain;

void handle_client(chat cl){
    cl.get_inp_message();
    cl.send_message();
}

void display_client(chat cl){
    cl.receive_message();
    cl.display_received();
}

int main() {
    chat cl;
    boost::thread cl_thread{handle_client},s_thread{display_client};
    cl_thread.join();
    s_thread.join();
    return 0;
}
