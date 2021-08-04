cmake_minimum_required(VERSION 3.16)
project(SeaBattleClient)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_MULTITHREADED  ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS system thread REQUIRED)

if(Boost_FOUND)
    set(H headers/)
    set(S sources/)
    list(APPEND HEADERS ${H}Logger.h ${H}SeaBattleGame.h ${H}Server.h)
    list(APPEND SOURCES ${S}main.cpp ${S}Logger.cpp ${S}SeaBattleGame.cpp ${S}Server.cpp)
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
    target_include_directories(${PROJECT_NAME} PUBLIC ${H})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
else()
    message("Boost doesn`t found, fatal error")
endif()