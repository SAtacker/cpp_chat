#pragma once

#include <boost/asio.hpp>

namespace{
    class server{
        private:
            std::string incomming_message;

        public:
            void receive_message();
    };
}