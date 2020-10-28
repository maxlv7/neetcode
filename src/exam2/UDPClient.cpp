//
// Created by itsuy on 2020/6/2.
//
#include <iostream>
#include "WinsockInit.h"
#include <WinSock2.h>

using namespace std;

int main() {
    cout << "client" << endl;
    WinsockInit::SockInit();

    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == INVALID_SOCKET) {
        cout << "socket fail!" << endl;
        return 0;
    }
    sockaddr_in sin{};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(7777);
    sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    while (true) {
        char buff[1024];
        cin >> buff;
        sendto(s, buff, strlen(buff), 0, (sockaddr *) &sin, sizeof(sin));
    }

    WinsockInit::SockFree();
}
