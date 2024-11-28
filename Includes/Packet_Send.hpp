#ifndef PACKET_SEND_HPP
#define PACKET_SEND_HPP

#include "Libraries.hpp"

class PacketSend {
    private:
        std::string ETHDevice;
        int Socket;
        uint8_t *PayloadData;
        size_t PayloadLen;
        char ETHPacket[4096];
        struct sockaddr_in DestInfo;
        struct iphdr *iph_send = (struct iphdr *)ETHPacket;
        struct udphdr *UDPHeader = (struct udphdr *)(ETHPacket + sizeof(struct iphdr));
        int PacketID = 0;

        void CreateSocket(const std::string& Device);
        void CloseSocket();


        void CreateIPHeader(const std::string& SourceIP, const std::string& DestIP, size_t PayloadLen);
        void CreateUDPHeader(int SourcePort, int DestPort, size_t PayloadLen);

    public:
        PacketSend(const std::string& ETHDevice);
        ~PacketSend();
        void FetchPacketFromQueue();

        void CreatePacket(const std::string& SourceIP, const std::string& DestIP, int SourcePort, int DestPort);

        void SendPacket();
};


#endif