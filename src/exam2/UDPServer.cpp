//
// Created by itsuy on 2020/6/2.
//
#include <iostream>
#include "WinsockInit.h"
#include <WinSock2.h>
#include <string>
using namespace std;

int main() {
    WinsockInit::SockInit();

    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == INVALID_SOCKET) {
        cout << "socket fail!" << endl;
        return 0;
    }
    //construction addr
    sockaddr_in sin{};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(7777);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;

    //bind port
    if (bind(s, (sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR) {
        cout << "bind error!" << endl;
    }

    //recv buffer
    char buff[1024];
    sockaddr_in addr{};
    int len = sizeof(addr);
    while (true) {
        //recv data
        auto rec = recvfrom(s, buff, sizeof(buff) - 1, 0, (sockaddr *) &addr, &len);
        if (rec > 0) {
            buff[rec] = '\0';
            auto data = inet_ntoa(addr.sin_addr);
            cout << data << endl;
            cout << buff << endl;
        }
        //judge data if data is "bye" then quit
        string s1{buff};
        if (s1.compare(string("bye"))==0) {
            cout << "bye" << endl;
            break;
        }
    }

    WinsockInit::SockFree();
}
