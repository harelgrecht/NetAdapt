#ifndef PACKET_CAPTURE_HPP
#define PACKET_CAPTURE_HPP

#include "Queue.hpp"
#include "Network_Config.hpp"

#include <string>
#include <iostream>
#include <pcap.h>
#include <vector>
#include <cstdint>


class PacketCapture {
    public:
        PacketCapture(NetworkConfig& networkInterface);
        ~PacketCapture();

        bool StartCapture(const std::string& FilterString);
    
        static Queue ReciveQueue;

    private:

        NetworkConfig& ethInterface;
        char ErrBuffer[PCAP_ERRBUF_SIZE];
        pcap_t* NetworkDescriptor;


        bool PcapDeviceOperation();
        bool SetPcapFilter(const std::string& FilterString);

        static void RecivePacketHandler(uint8_t* GlobalData, const struct pcap_pkthdr* PacketHeader, const uint8_t* PacketData);
};

//MockUp functions
void CreateSimulatedPackets(std::vector<std::vector<uint8_t>>& packets);
void SimulatePacketInjector();


#endif