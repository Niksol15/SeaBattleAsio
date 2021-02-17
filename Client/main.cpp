#include <iostream>
#include "Client.h"

using namespace std;

constexpr int kDefaultPort = 1235;
constexpr char kDefaultIp[] = "127.0.0.1";

int main() {
    Client client;
    if (client.connect(kDefaultIp, kDefaultPort)) {
        cout << "Connected\n";
    } else {
        cout << "Problems with connecting to the server\n";
        return 0;
    }
    string str;
    while (true) {
        getline(cin, str);
        if (!client.send(str)) {
            cout << "Problem with the sending message arose";
            break;
        }
        cout << client.receive();
    }
    return 0;
}
