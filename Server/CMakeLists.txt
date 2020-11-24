cmake_minimum_required(VERSION 3.17)
project(Server)

set(CMAKE_CXX_STANDARD 17)

set(ASIO_ROOT D:/SDK/asio-1.18.0/include)

include_directories(${ASIO_ROOT})

link_libraries(ws2_32 wsock32)

add_executable(Server main.cpp)