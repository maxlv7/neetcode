#include "Scanner.h"
#include <WinSock2.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;


bool Scanner::connScan(std::string ip, unsigned short port) {
    cout << "scan: " << port << endl;
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) {
        cout << "socket fail!" << endl;
        return false;
    }
    //addr sturct
    sockaddr_in sin{};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.S_un.S_addr = inet_addr(std::move(ip).c_str());

    int code = connect(s, (sockaddr *) &sin, sizeof(sin));
    if (code == SOCKET_ERROR) {
        cout << "connect error!" << endl;
        return false;
    } else {
        cout << port << "alive..." << endl;
    }
    closesocket(s);
    return false;
}

tuple<unsigned int, unsigned int> Scanner::parsePort(std::string portRange) {
    std::string s = std::move(portRange);
    auto pos = s.find(":");
    //str to int
    unsigned int start = std::strtoul(s.substr(0,pos).c_str(), nullptr,10);
    unsigned int end = std::strtoul(s.substr(pos+1,s.size()-pos-1).c_str(), nullptr,10);
    return std::make_tuple(start, end);
}
//TODO add domain support
bool Scanner::portScan(std::string ip, unsigned short port) {
    auto res = gethostname(const_cast<char *>(ip.c_str()),ip.size());
    return false;
}
