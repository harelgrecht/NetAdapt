#ifndef PACKET_SEND_HPP
#define PACKET_SEND_HPP

#include "Packet_Process.hpp"
#include "Network_Config.hpp"

#include <string>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>


class PacketSend {

    public:
        PacketSend(NetworkConfig& config, const std::string& DestIP, const int& DestPort);
        ~PacketSend();

        void SendPacket();
        void CreatePacket();
        void FetchPacketFromQueue();
        
    private:
        
        void CreateIPHeader();
        void CreateUDPHeader();

        char ETHPacket[4096];
        NetworkConfig& ethInterface;
        int socketFD;
        uint8_t* payloadData;
        size_t payloadLen;
        int PacketID = 0;
        struct iphdr *iph_send = (struct iphdr *)ETHPacket;
        struct udphdr *UDPHeader = (struct udphdr *)(ETHPacket + sizeof(struct iphdr));
        struct sockaddr_in destInfo;
        std::string DestIP;
        int DestPort;
};

#endif