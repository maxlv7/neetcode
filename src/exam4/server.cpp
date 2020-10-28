/**
 * @author  Li
 * @date  2020/6/18 18:03
 * @version 1.0
 */
#include "WinsockInit.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <thread>

int main(int argc, char **argv) {
    WinsockInit::SockInit();
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sin{};
    sin.sin_port = htons(7777);
    sin.sin_family = AF_INET;
    const char *ip = "127.0.0.1";
    inet_pton(AF_INET, ip, &sin.sin_addr);
    int b = bind(s, (sockaddr *) &sin, sizeof(sin));
    if (b != 0) {
        std::cout << "bind error!" << std::endl;
    }
    int ret = listen(s, 10);
    if (ret != 0) {
        std::cout << "listen error!" << std::endl;
    }
    int count = 0;
    while (true) {
        sockaddr_in addr_client{};
        int sock_size = sizeof(addr_client);
        SOCKET sac = accept(s, (sockaddr *) &addr_client, &sock_size);
        cout << "new socket" << sac;
        //get client addr
        char cip[20];
        auto addr = inet_ntop(AF_INET, &addr_client.sin_addr, cip, sizeof(cip));
        std::cout << " from" << addr << std::endl;
        count+=1;
        std::cout << "All connection:" << count << endl;
        if (sac == INVALID_SOCKET) {
            std::cout << "error@accept!";
        } else {
            // start new thread
            std::thread t([&count,sac, s, addr_client]() {
                while (true) {
                    char buf[1024];
                    int len = recv(sac, buf, sizeof(buf) - 1, 0);
                    if (len > 0) {
                        buf[len] = '\0';
                        send(sac, buf, len, 0);
                        std::cout << buf << std::endl;

                        if (buf[0] == 'q') {
                            std::cout << "quit" << std::endl;
                            closesocket(sac);
                        }
                    } else {
                        count-=1;
                        std::cout << "All connection:" << count << endl;
                        break;
                    }
                }
            });
            t.detach();
        }
    }


    WinsockInit::SockFree();
    return 0;
}


