#pragma once

#include <boost/asio.hpp>

namespace libmain {
    std::string helloWorld();
    std::string get_message(boost::asio::ip::tcp::socket& socket );
    void send_message(boost::asio::ip::tcp::socket& socket , std::string& message);
    std::string incoming_message();
}
