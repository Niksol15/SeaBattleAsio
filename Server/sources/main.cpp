#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <string_view>
#include <exception>
#include <stdexcept>
#include <sstream>

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

using namespace asio;
using namespace std::chrono;

constexpr int PORT = 27015;
const std::string ABOUT = "Author: Nikita Solonko K-26";
const std::string DISCONNECT = "You have been disconnected";
const std::string UNKNOWN = "Unknown command, please try again";

enum class RequestType{
    Shot,
    Disconnect,
    Statistic,
    NewGame,
    Who,
    Empty,
    Unknown
};

class SeaBattleGame{
public:
    SeaBattleGame(): battleField(DEFAULT_BATTLEFIELD), aliveCellCounter(0), shotCounter(0), hitCounter(0){
        for(const auto& row: battleField){
            for(const auto& cell: row){
                if(cell == 1){
                    ++aliveCellCounter;
                }
            }
        }
    }
    std::string Shot(int x, int y){
        ++shotCounter;
        std::cout << x << " " << y << std::endl;
        if( x >= 0 && x < battleField.size() && y >= 0 && y < battleField[x].size()){
            if(battleField[x][y] == 1){
                battleField[x][y] = -1;
                --aliveCellCounter;
                ++hitCounter;
                return  aliveCellCounter == 0 ? "You win" : "You hit\n" + battlefieldToString();
            } else if(battleField[x][y] == 0){
                battleField[x][y] = -2;
                return "You missed\n" + battlefieldToString();
            } else {
                return "You missed, this cell already hit\n" + battlefieldToString();
            }
        }
        return "You missed, out of range";
    }

    std::string Statistic(){
        return "Your statistic: " + std::to_string(hitCounter) + " hit, " + std::to_string(shotCounter) + " shot";
    }

    std::string NewGame(){
        battleField = DEFAULT_BATTLEFIELD;
        aliveCellCounter = 0;
        shotCounter = 0;
        hitCounter = 0;
        for(const auto& row: battleField) {
            for (const auto &cell: row) {
                if (cell == 1) {
                    ++aliveCellCounter;
                }
            }
        }
        return "You are starting new game";
    }
    std::string battlefieldToString(){
        std::string res;
        res.reserve((battleField.size() + 2) * (battleField[0].size() + 2));
        res += " ---------- \n";
        for(const auto& row: battleField){
            res += '|';
            for(const auto& cell: row){
                if(cell == 1 || cell == 0){
                    res += ' ';
                } else if(cell == -1){
                    res += '*';
                } else {
                    res += '0';
                }
            }
            res += "|\n";
        }
        res += " ---------- \n";
        return res;
    }
private:
    std::vector<std::vector<int>> battleField;
    int aliveCellCounter;
    int shotCounter;
    int hitCounter;
};

class Server{
public:
    Server(const std::string& ip, int port): endpoint(asio::ip::make_address(ip, ec), port),
                                    socket(context), acceptor(context, endpoint), logger("Server Created"), seaBattleGame()
    {
        acceptor.accept(socket);
        if(!ec){
            logger.Log("Client Connected");
        }else{
            logger.Log(ec.message());
            throw std::runtime_error("Problem with connection");
        }
    }
    RequestType Process(){
        socket.wait(socket.wait_read);
        size_t bytes = socket.available();
        std::vector<char> vBuffer(bytes);
        requestType = RequestType::Empty;
        if(bytes) {
            socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);
            std::string sRequest;
            sRequest.reserve(vBuffer.size());
            for (char ch: vBuffer) {
                sRequest += ch;
            }
            logger.Log("Accept Request: \"" + sRequest + '\"');
            std::string sResponse = processRequest(sRequest);
            socket.write_some(asio::buffer(sResponse.data(), sResponse.size()));
            return requestType;
        }
        return requestType;
    }
private:
    std::string processRequest(const std::string& request){
        std::istringstream request_stream(request);
        std::string command;
        request_stream >> command;
        if(command == "Who"){
            requestType = RequestType::Who;
            return ABOUT;
        } else if (command == "shot"){
            requestType = RequestType::Shot;
            int x, y;
            request_stream >> x >> y;
            return seaBattleGame.Shot(x, y);
        } else if(command == "statistic"){
            requestType = RequestType::Statistic;
            return seaBattleGame.Statistic();
        } else if (command == "new"){
            std::string second_part;
            request_stream >> second_part;
            if(second_part == "game"){
                requestType = RequestType::NewGame;
                return seaBattleGame.NewGame();
            }  else {
                requestType = RequestType::Unknown;
                return UNKNOWN;
            }
        }else if(command == "disconnect"){
            requestType = RequestType::Disconnect;
            return DISCONNECT + ", " + seaBattleGame.Statistic();
        } else {
            requestType = RequestType::Unknown;
            return UNKNOWN;
        }
    }
    io_context context;
    error_code ec;
    ip::tcp::endpoint endpoint;
    ip::tcp::socket socket;
    ip::tcp::acceptor acceptor;
    Logger logger;
    SeaBattleGame seaBattleGame;
    RequestType requestType = RequestType::Empty;
};

int main(){
    try{
        Server server("127.0.0.1", PORT);
        RequestType request;
        do{
            request = server.Process();
        } while (request != RequestType::Disconnect);
    } catch (std::exception&) {
        Logger("Problem with connection");
    }
    return 0;
}
/*
int main() {
    error_code ec;
    ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), PORT);
    ip::tcp::socket socket(context);
    ip::tcp::acceptor acceptor(context, endpoint);
    acceptor.accept(socket);
    if(!ec){
        std::cout << "Client Connected" << std::endl;
    }else{
        std::cout << ec.message();
    }
    while(true){
        socket.wait(socket.wait_read);
        size_t bytes = socket.available();
        std::vector<char> vBuffer(bytes);
        if(bytes) {
            std::cout << "Accept command" << std::endl;
            socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);
            for (char ch: vBuffer) {
                std::cout << ch;
            }
            std::cout << std::endl;
        }
        Command request = stringToCommand({vBuffer.data(), vBuffer.size()});
        std::string sResponse = processCommand(request);
        socket.write_some(asio::buffer(sResponse.data(), sResponse.size()));
        if(request == Command::Disconnect){
            break;
        }
    }

    return 0;
}
*/
/*
int main() {
    asio::error_code ec;
    asio::io_context context;
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), 3213);
    asio::ip::tcp::socket socket(context);
    socket.connect(endpoint);
    if(!ec){
        std::cout << "Connected" << std::endl;
    }else{
        std::cout << ec.message();
    }
    socket.wait(socket.wait_read);
    size_t bytes = socket.available();
    if(bytes) {
        std::cout << "read" << std::endl;
        std::vector<char> vBuffer(bytes);
        socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);
        for (char ch: vBuffer) {
            std::cout << ch;
        }
    }
    return 0;
}*/

/*
#include <stdio.h>
#include "winsock2.h"

int main() {
    //----------------------
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR)
        printf("Error at WSAStartup()\n");

    //----------------------
    // Create a SOCKET for listening for
    // incoming connection requests.
    SOCKET ListenSocket;
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }

    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(27015);

    if (bind( ListenSocket,
              (SOCKADDR*) &service,
              sizeof(service)) == SOCKET_ERROR) {
        printf("bind() failed.\n");
        closesocket(ListenSocket);
        return 0;
    }

    //----------------------
    // Listen for incoming connection requests
    // on the created socket
    if (listen( ListenSocket, 1 ) == SOCKET_ERROR)  //можливе лише одне підключення
        printf("Error listening on socket.\n");

    printf("Listening on socket...\n");
    WSACleanup();
    return 0;
}
*/