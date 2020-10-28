/**
 * @author  Li
 * @date  2020/6/3 15:37
 * @version 1.0
 */
#pragma once

#include <string>
#include <tuple>

using std::tuple;
class Scanner {
public:
    bool connScan(std::string ip,unsigned short port);
    bool portScan(std::string ip,unsigned short port);
    static tuple<unsigned int,unsigned int > parsePort(std::string portRange);
};




