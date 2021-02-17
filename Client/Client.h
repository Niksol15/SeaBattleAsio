//
// Created by niksol on 16.02.2021.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <string>
#include <cstdint>

class Client {
public:
    Client();

    bool connect(const std::string& address, uint16_t port);

    bool send(const std::string & request);

    std::string receive();

    void disconnect();

    ~Client();
private:
    boost::system::error_code m_ec;
    boost::asio::io_context m_context;
    boost::asio::ip::tcp::socket m_socket;
};


#endif //CLIENT_H
