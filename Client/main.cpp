#include <iostream>
#include "Client.h"

using namespace std;

constexpr int kDefaultPort = 1235;
constexpr char kDefaultIp[] = "127.0.0.1";

int main() {
    Client client;
    if(client.connect(kDefaultIp, kDefaultPort)){
        cout << "Connected\n";
    } else {
        cout << "Problem with connection\n";
        return 0;
    }
    while (true){
        string str;
        getline(cin, str);
        if(!client.send(str) || str == "dei"){
            cout << "Crashed\n";
            break;
        }
        cout << client.receive();
    }
    return 0;
}
