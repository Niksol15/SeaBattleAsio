//
// Created by niksol on 17.02.2021.
//

#ifndef SERVER_LOGGER_H
#define SERVER_LOGGER_H

#include <chrono>
#include <fstream>
#include <string>

class Logger {
public:
    Logger() noexcept;

    void log(const std::string &msg) noexcept;

private:
    std::ofstream m_log;
    std::chrono::steady_clock::time_point start;
};


#endif //SERVER_LOGGER_H
