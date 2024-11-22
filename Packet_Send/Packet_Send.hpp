#ifndef PACKET_SEND_HPP
#define PACKET_SEND_HPP

#include "../MUST.HPP"


class PacketSend {
    private:
        int Socket;
        void CreateSocket();
        void CloseSocket();
        
    public:
        PacketSend();
        ~PacketSend();

        void SendPacket(const std::string& DestIP, const int& DestPort, const uint8_t& PayloadData, size_t PayloadLen);

};


#endif