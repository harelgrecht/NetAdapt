#ifndef PACKET_CAPTURE_HPP
#define PACKET_CAPTURE_HPP

#include <iostream>
#include <pcap.h>
#include <vector>
#include "../Queue/Queue.hpp"


#define MAX_PACKET_SIZE 360
#define PROMISC 1
#define READ_TIMOUT 1000 // If no packets recived in 1000ms
#define LOOP_COUNT -1

class PacketCapture {
    public:
        PacketCapture(const std::string& EthDevice, std::string IpAddress);
        ~PacketCapture();

        bool StartCapture();
    
    private:
        static void packetHandler(u_char* GlobalData, const struct pcap_pkthdr* PacketHeader, const u_char* PacketData);
        bool SetIPAddress(const std::string& device, const std::string& IpAddress);

        bool OpenDevice();


        char ErrBuffer[PCAP_ERRBUF_SIZE];
        std::string Device;
        std::string IpAddress;
        static Queue ReciveQueue;
        pcap_t* Handle;
};




#endif