#ifndef PACKET_CAPTURE_HPP
#define PACKET_CAPTURE_HPP

#include "Queue.hpp"

#include <string>
#include <iostream>
#include <pcap.h>
#include <vector>
#include <cstdint>


class PacketCapture {
    public:
        PacketCapture(const std::string& EthDevice, std::string IpAddress);
        ~PacketCapture();

        bool StartCapture(const std::string& FilterString);
    
        static Queue ReciveQueue;

    private:
        static void RecivePacketHandler(uint8_t* GlobalData, const struct pcap_pkthdr* PacketHeader, const uint8_t* PacketData);
        bool SetIPAddress(const std::string& device, const std::string& IpAddress);

        bool OpenDevice();
        bool SetFilter(const std::string& FilterString);


        char ErrBuffer[PCAP_ERRBUF_SIZE];
        std::string Device;
        std::string IpAddress;
        pcap_t* NetworkDescriptor;

};

//MockUp functions
void CreateSimulatedPackets(std::vector<std::vector<uint8_t>>& packets);
void SimulatePacketInjector();


#endif