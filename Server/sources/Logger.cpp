//
// Created by niksol on 17.02.2021.
//

#include "Logger.h"

using namespace std::chrono;

Logger::Logger() noexcept: m_log("Log.txt"), start(steady_clock::now()) {
}

void Logger::log(const std::string &msg) noexcept {
    m_log << msg << " (time point = " << duration_cast<seconds>(steady_clock::now() - start).count()
    << ")" << std::endl;
}
