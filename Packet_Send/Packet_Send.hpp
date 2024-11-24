#ifndef PACKET_SEND_HPP
#define PACKET_SEND_HPP

#include "../MUST.HPP"


class PacketSend {
    private:
        int Socket;
        uint8_t *Payload;
        size_t PayloadLen;
        void CreateSocket();
        void CloseSocket();
        
    public:
        PacketSend();
        ~PacketSend();
        void GetPackets();


        void SendPacket(const std::string& DestIP, const int& DestPort);

};


#endif