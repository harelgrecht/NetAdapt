#ifndef PACKET_SEND_HPP
#define PACKET_SEND_HPP

#include "Libraries.hpp"

class PacketSend {
    private:
        int Socket;
        uint8_t *PayloadData;
        size_t PayloadLen;
        void CreateSocket();
        void CloseSocket();
        
    public:
        PacketSend();
        ~PacketSend();
        void GetPackets();

        void BindSocketToInterface(const std::string &DeviceName);

        void SendPacket(const std::string& DestIP, const int& DestPort, const std::string& destDevice);

};


#endif