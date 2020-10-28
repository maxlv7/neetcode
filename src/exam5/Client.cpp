//
// Created by Li on 2020/6/20.
//
#include <iostream>
#include "WinsockInit.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

int main(int argc,char **argv){
    WinsockInit::SockInit();

    WinsockInit::SockFree();
    return 0;
}
