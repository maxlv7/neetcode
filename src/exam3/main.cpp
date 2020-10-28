//
// Created by Li on 2020/6/3.
//
#include <iostream>
#include "WinsockInit.h"
#include "Scanner.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    WinsockInit::SockInit();
    auto hint = R"(usage:PortScan.exe ip port_start:port_end)";
    auto example = R"(usage:PortScan.exe 127.0.0.1 5555:6666)";
    if (argc < 3) {
        cout << hint << endl;
        cout << example << endl;
    } else {
        Scanner scanner;
        //parse port range
        const auto[start, end] = Scanner::parsePort(std::string(argv[2]));
        for (auto i = start; i <= end; i++) {
            //start scan
            scanner.connScan(argv[1], i);
        }
    }
    WinsockInit::SockFree();
    return 0;
}
