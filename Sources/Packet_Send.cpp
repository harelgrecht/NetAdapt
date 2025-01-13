#include "../Includes/Packet_Send.hpp"
#include "../Includes/Global_Defines.hpp"
#include "../Includes/Queue.hpp"

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <arpa/inet.h>

PacketSend::PacketSend(NetworkConfig& config, const std::string& DestIP, const int& DestPort)
    : ethInterface(config), payloadData(new uint8_t[PACKET_SIZE]), payloadLen(0), DestIP(DestIP), DestPort(DestPort) {
    }

PacketSend::~PacketSend() {
#ifndef MOCK_UP
    delete[] payloadData;
#endif
}

void PacketSend::CreateIPHeader() {
    iph_send->ihl = IP_HEADER_LENGTH_UNITS;
    iph_send->version = IP_VERSION;
    iph_send->tos = 0;
    iph_send->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + payloadLen;
    iph_send->id = htons(PacketID++);
    iph_send->frag_off = 0;
    iph_send->ttl = DEFAULT_TTL_VALUE;
    iph_send->protocol = IPPROTO_UDP;
    iph_send->check = 0;
    iph_send->saddr = inet_addr(ethInterface.getIPAddress().c_str());
    iph_send->daddr = inet_addr(DestIP.c_str());
}


void PacketSend::CreateUDPHeader() {
    UDPHeader->source = htons(ethInterface.getPort());
    UDPHeader->dest = htons(DestPort);
    UDPHeader->len = htons(sizeof(struct udphdr) + payloadLen);
}

void PacketSend::CreatePacket() {
    memcpy(ETHPacket + sizeof(struct iphdr) + sizeof(struct udphdr), payloadData, payloadLen);
    CreateIPHeader();
    CreateUDPHeader();

    memset(&destInfo, 0, sizeof(destInfo));
    destInfo.sin_family = AF_INET;
    destInfo.sin_port = htons(DestPort);
    destInfo.sin_addr.s_addr = inet_addr(DestIP.c_str());
}

void PacketSend::FetchPacketFromQueue() {
    if(PacketProcess::SendQueue.dequeue(payloadData, payloadLen) == false) {
        std::cout << "Failed to get a packet from the SendQueue" << std::endl;
    }
}

void PacketSend::SendPacket() {
#ifndef MOCK_UP
    FetchPacketFromQueue();
    CreatePacket();
    int socketFD = ethInterface.getSocketFD();
    if(sendto(socketFD, ETHPacket, iph_send -> tot_len, 0, (struct sockaddr *)&destInfo, sizeof(destInfo)) < 0)
        std::cerr << "Failed sending the packet: " << PacketID << " Error: " << std::strerror(errno) << std::endl;
#endif
}