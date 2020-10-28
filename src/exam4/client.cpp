/**
 * @author  Li
 * @date  2020/6/18 18:03
 * @version 1.0
 */
#include "WinsockInit.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
//    const char* ipip = argv[1];

    WinsockInit::SockInit();
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sin{};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(7777);
    const char *ip = "127.0.0.1";
    inet_pton(AF_INET, ip, &sin.sin_addr);

    int ret = connect(s, (sockaddr *) &sin, sizeof(sin));
    if (ret != 0) {
        cout << "connect error!" << endl;
    }
    char buf[20];
    cin >> buf;
    send(s, buf, strlen(buf), 0);
    char buff[1024];
    int len = recv(s, buff, sizeof(buff),0);
    buff[len]='\0';
    cout << buff << endl;
    WinsockInit::SockFree();
    return 0;
}


