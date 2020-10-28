/**
 * @author  Li
 * @date  2020/6/2 22:01
 * @version 1.0
 */
#pragma once

#include <WinSock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;

class WinsockInit {
public:
    static void SockInit();

    static void SockFree();
};

void WinsockInit::SockInit() {
    WORD wVersionRequested;
    WSADATA wsaData;

    wVersionRequested = MAKEWORD(2, 2);
    auto err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        cout << "WSAStart start error" << err << endl;
    }
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        cout << "can not find Winsock.dll" << endl;
        WSACleanup();
    } else {
        cout << "Winsock 2.2 dll load success!" << endl;
    }
}

void WinsockInit::SockFree() {
    WSACleanup();
}




