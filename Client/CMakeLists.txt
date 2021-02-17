cmake_minimum_required(VERSION 3.16)
project(SeaBattleClient)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_MULTITHREADED  ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS system thread REQUIRED)

if(Boost_FOUND)
    add_executable(${PROJECT_NAME} main.cpp Client.h Client.cpp)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
else()
    message("Boost doesn`t found, fatal error")
endif()


