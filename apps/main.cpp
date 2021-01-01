#include "libmain/client.hpp"
#include "libmain/server.hpp"

using namespace std;
using namespace libmain;

int main(int argc,char* argv[]) {
    cout<<"=================================== Welcome ========================================"<<endl;
    if(argc == 4){
        client c{argv[1],stoi(argv[2]),argv[3]};
        while (true)
        {
            c.get_inp_message();
            c.send_message();
            if(c.is_quit()){
                break;
            }
        }
    }else if (argc==3)
    {
        server s{argv[1],(stoi(argv[2]))};
        s.init_server();
    }
    else{
        cout<<"===============Entered sufficient arguments================="<<endl;
        cout<<"ip port username"<<endl;
    }
    cout<<"=================================== Thank You ========================================"<<endl;
    return 0;
}
