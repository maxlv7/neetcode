//
// Created by Li on 2020/6/20.
//
#include <iostream>
#include "WinsockInit.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>

using namespace std;

struct socket_information {
    char buffer[1024];
    WSABUF dataBuf;
    SOCKET socket;
    unsigned long bytesSend;
    unsigned long bytesRecv;
};

void createSocketInformation(int &total_sockets, SOCKET s, socket_information *socketArray[]) {
    auto *si = new socket_information();
    si->socket = s;
    si->bytesSend = 0;
    si->bytesRecv = 0;
    socketArray[total_sockets] = si;
    total_sockets++;
}

void freeSocketInformation(int index, int &total_sockets, socket_information *socketArray[],
                           socket_information *info) {
    cout << "successful close->" << info->socket << endl;
    closesocket(info->socket);
    delete info;
    info = nullptr;
    for (int i = index; i < total_sockets; i++) {
        socketArray[i] = socketArray[i + 1];
    }
    total_sockets--;
}

int main(int argc, char **argv) {
    WinsockInit::SockInit();

    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // addr struct
    sockaddr_in sin{0};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(7777);
    inet_pton(AF_INET, "0.0.0.0", &sin.sin_addr);

    auto ret = bind(s, (sockaddr *) &sin, sizeof(sin));
    if (ret == SOCKET_ERROR) {
        cout << "bind error!" << endl;
        return -1;
    }
    if (listen(s, 10)) {
        cout << "listen error!" << endl;
        return -1;
    }
    unsigned long val = 1;
    // set non block
    if (ioctlsocket(s, FIONBIO, &val) == SOCKET_ERROR) {
        cout << "ioctl error!" << endl;
        return -1;
    }
    // Data
    fd_set read_set;
    fd_set write_set;
    int total = 0;
    int total_sockets = 0;
    socket_information *socketArray[64];

    createSocketInformation(total_sockets, s, socketArray);
    while (true) {
        FD_ZERO(&read_set);
        FD_ZERO(&write_set);

        FD_SET(s, &read_set);
        //给socket array里面的每个socket设置到fd_set里面去
        for (int i = 0; i < total_sockets; ++i) {
            socket_information *socketInformation = socketArray[i];
            FD_SET(socketInformation->socket, &read_set);
            FD_SET(socketInformation->socket, &write_set);
        }
        total = select(0, &read_set, &write_set, nullptr, nullptr);
        if (total == SOCKET_ERROR) {
            cout << "select error!" << WSAGetLastError() << endl;
            return -1;
        }
//        cout << total << endl;
        for (int j = 0; j < total_sockets; j++) {
            auto info = socketArray[j];
            if (FD_ISSET(info->socket, &read_set)) {
                if (info->socket == s) {
                    total--;
                    SOCKET acceptSocket;
                    sockaddr_in accept_sin{};
                    int accept_sin_len = sizeof(accept_sin);
                    if ((acceptSocket = accept(s, (sockaddr *) &accept_sin,
                                               &accept_sin_len)) != INVALID_SOCKET) {
                        if (ioctlsocket(acceptSocket, FIONBIO, &val) == SOCKET_ERROR) {
                            cout << "ioctl error!" << endl;
                            return -1;
                        }
                        {
                            char ip[20];
                            auto src = inet_ntop(AF_INET, &accept_sin.sin_addr, ip, sizeof(ip));
                            auto port = ntohs(accept_sin.sin_port);
                            cout << "ID: (" << acceptSocket << ") new connection from:" << src << ":" << port << endl;
                        }
                        createSocketInformation(total_sockets, acceptSocket, socketArray);
                    } else {
                        cout << "accept error!" << WSAGetLastError() << endl;
                    }
                } else {
                    if (FD_ISSET(info->socket, &read_set)) {
                        total--;
//                        cout << "accept socket" << endl;
                        char b[4096];
                        int len = recv(info->socket, b, sizeof(b)-1, 0);
                        if (len == 0) {
                            freeSocketInformation(j, total_sockets, socketArray, info);
                        }
//                        if(WSAGetLastError()!=WSAEWOULDBLOCK){
//                            cout << "socket close" << endl;
//                        }
                        b[len] = '\0';
//                        cout << len << endl;
                        if (b[0] == 'b' && b[1] == 'y' && b[2] == 'e') {
                            exit(0);
                        }
                        cout << b << endl;
                    }
                }
            } else {
                if (FD_ISSET(info->socket, &write_set)) {
                    {
//                        cout << "send(" << info->socket << ")" << endl;
                        char buff[1024];
//                        cin >> buff;
//                        int len = send(info->socket, buff, strlen(buff), 0);
                    }
                }
//                cout << info->socket << " not in read_set" << endl;
            }
        }
//        this_thread::sleep_for(3s);
    }
    WinsockInit::SockFree();
    return 0;
}

