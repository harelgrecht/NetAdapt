#include "../Includes/Packet_Send.hpp"
#include "../Includes/Global_Defines.hpp"

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <arpa/inet.h>

PacketSend::PacketSend(const std::string& Device) : ETHDevice(Device), PayloadData(nullptr), PayloadLen(0) {
    PayloadData = new uint8_t[PACKET_SIZE];
    CreateSocket(ETHDevice.c_str());
}

PacketSend::~PacketSend() {
    CloseSocket();
    delete[] PayloadData;
}

void PacketSend::CreateSocket(const std::string& Device) {
    Socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (Socket < 0)
        throw std::runtime_error("Failed to create socket: " + std::string(std::strerror(errno)));

    if (setsockopt(Socket, SOL_SOCKET, SO_BINDTODEVICE, Device.c_str(), Device.length() + 1) < 0) {
        close(Socket);
        throw std::runtime_error("Faild to bind socket: " + std::string(std::strerror(errno)));
    }
}

void PacketSend::CloseSocket() {
    if (Socket >= 0)
        close(Socket);
}

void PacketSend::CreateIPHeader(const std::string& SourceIP, const std::string& DestIP, size_t PayloadLen) {
    iph_send->ihl = IP_HEADER_LENGTH_UNITS;
    iph_send->version = IP_VERSION;
    iph_send->tos = 0;
    iph_send->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + PayloadLen;
    iph_send->id = htons(PacketID++);
    iph_send->frag_off = 0;
    iph_send->ttl = DEFAULT_TTL_VALUE;
    iph_send->protocol = IPPROTO_UDP;
    iph_send->check = 0; 
    iph_send->saddr = inet_addr(SourceIP.c_str());
    iph_send->daddr = inet_addr(DestIP.c_str());
}


void PacketSend::CreateUDPHeader(int SourcePort, int DestPort, size_t PayloadLen) {
    UDPHeader->source = htons(SourcePort);
    UDPHeader->dest = htons(DestPort);
    UDPHeader->len = htons(sizeof(struct udphdr) + PayloadLen);
}

void PacketSend::CreatePacket(const std::string& SourceIP, const std::string& DestIP, int SourcePort, int DestPort) {
    memcpy(ETHPacket + sizeof(struct iphdr) + sizeof(struct udphdr), PayloadData, PayloadLen);
    CreateIPHeader(SourceIP, DestIP, PayloadLen);
    CreateUDPHeader(SourcePort, DestPort, PayloadLen);

    memset(&DestInfo,0 ,sizeof(DestInfo));
    DestInfo.sin_family = AF_INET;
    DestInfo.sin_port = htons(DestPort);
    DestInfo.sin_addr.s_addr = inet_addr(DestIP.c_str());
}

void PacketSend::FetchPacketFromQueue() {
    if(PacketProcess::SendQueue.dequeue(PayloadData, PayloadLen) == false) {
        std::cout << "Failed to get a packet from the SendQueue" << std::endl;
    }
}

void PacketSend::SendPacket(const std::string& SourceIP, const std::string& DestIP, int SourcePort, int DestPort) {
    FetchPacketFromQueue();
    CreatePacket(SourceIP, DestIP, SourcePort, DestPort); 
    if(sendto(Socket, ETHPacket, iph_send -> tot_len, 0, (struct sockaddr *)&DestInfo, sizeof(DestInfo)) < 0)
        std::cerr << "Failed sending the packet: " << PacketID << " Error: " << std::strerror(errno) << std::endl;
}