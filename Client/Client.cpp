//
// Created by niksol on 16.02.2021.
//

#include "Client.h"
#include <boost/asio/buffer.hpp>

namespace {
    constexpr char kProblemWithConnection[] = "There was a problem with the socket";
}

Client::Client() : m_socket(m_context) {}

bool Client::connect(const std::string &address, uint16_t port) {
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(address, m_ec), port);
    if(m_socket.is_open()){
        disconnect();
    }
    m_socket.connect(endpoint, m_ec);
    return !static_cast<bool>(m_ec);
}

bool Client::send(const std::string &request) {
    if (m_socket.is_open()) {
        m_socket.write_some(boost::asio::buffer(request), m_ec);
    }
    return !static_cast<bool>(m_ec) && m_socket.is_open();
}

std::string Client::receive() {
    m_socket.wait(m_socket.wait_read);
    std::size_t bytes = m_socket.available();
    if (bytes && !m_ec) {
        std::string response;
        response.resize(bytes);
        m_socket.read_some(boost::asio::buffer(response), m_ec);
        return response;
    } else {
        return kProblemWithConnection;
    }
}

void Client::disconnect() {
    if(m_socket.is_open()){
        m_socket.close(m_ec);
    }
}

Client::~Client() {
    disconnect();
}