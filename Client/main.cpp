#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

using namespace std::chrono;

constexpr int VARIANT = 16;
constexpr int PORT = 1025 + VARIANT;

class Logger{
public:
    Logger(): fout("Log.txt"), start(steady_clock::now()){
    }
    explicit Logger(const std::string& msg): fout("Log.txt"), start(steady_clock::now()){
        fout << msg << " (time point = " << duration_cast<seconds>(steady_clock::now() - start).count()  << ")" << std::endl;
    }
    void Log(const std::string& msg){
        fout << msg << " (time point = " << duration_cast<seconds>(steady_clock::now() - start).count()  << ")"<< std::endl;
    }
private:
    std::ofstream fout;
    std::chrono::steady_clock::time_point start;
};

int main() {
    asio::error_code ec;
    asio::io_context context;
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), 27015);
    asio::ip::tcp::socket socket(context);
    socket.connect(endpoint, ec);
    Logger logger;
    if(!ec){
        logger.Log("Connected");
    }else{
        logger.Log(ec.message());
        return 0;
    }
    if(socket.is_open()){
        logger.Log("Socket is open");
        std::string sRequest;
        do{
            std::getline(std::cin, sRequest);
            socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
            socket.wait(socket.wait_read);
            size_t bytes = socket.available();
            std::string sResponse;
            sResponse.reserve(bytes);
            if(bytes){
                std::vector<char> vBuffer(bytes);
                socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);
                for(char ch: vBuffer){
                    sResponse += ch;
                }
                logger.Log(sResponse);
            }
        }while (sRequest != "disconnect");
    }
    return 0;
}
