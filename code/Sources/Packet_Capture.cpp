#include "../Includes/Global_Defines.hpp"
#include "../Includes/Packet_Capture.hpp"
#include "../Includes/Exceptions.hpp"
#include "../Includes/Queue.hpp"
#include "../Includes/Network_Config.hpp"

#include <random>
#include <cstring>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <net/if.h>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>

Queue PacketCapture::ReciveQueue;

PacketCapture::PacketCapture(NetworkConfig& networkInterface)
    : ethInterface(networkInterface), NetworkDescriptor(nullptr) {}
    
PacketCapture::~PacketCapture() {
    if (NetworkDescriptor) 
        pcap_close(NetworkDescriptor);
}

bool PacketCapture::PcapDeviceOperation() {
#ifndef MOCK_UP
    NetworkDescriptor = pcap_open_live(ethInterface.getDeviceName().c_str(), PACKET_SIZE, PROMISC, CAPTURE_READ_TIMEOUT_MS, ErrBuffer);
    if (NetworkDescriptor == nullptr){
        throw NetworkException("Error opening device: " + std::string(ErrBuffer));
    }
#endif
    return true;
}

bool PacketCapture::SetPcapFilter(const std::string& FilterString) {
#ifndef MOCK_UP
    struct bpf_program bpf_filter;
    if(pcap_compile(NetworkDescriptor, &bpf_filter, FilterString.c_str(),0, PCAP_NETMASK_UNKNOWN) == -1) {
        pcap_freecode(&bpf_filter);
        throw NetworkException("Failed to compile filter: " + std::string(pcap_geterr(NetworkDescriptor)));
    }

    if (pcap_setfilter(NetworkDescriptor, &bpf_filter) == -1) {
        pcap_freecode(&bpf_filter);
        throw NetworkException("Failed to set filter: " + std::string(pcap_geterr(NetworkDescriptor)));
    }
    pcap_freecode(&bpf_filter);
    std::cout << "Filter applied: " << FilterString << std::endl;
#endif
    return true;
}

bool PacketCapture::StartCapture(const std::string& FilterString) {
    try {
        PcapDeviceOperation();
        SetPcapFilter(FilterString);
    } catch (const NetworkException& e) {
        std::cerr << "Error starting capture: " << e.what() << std::endl;
    }

#ifdef MOCK_UP
    SimulatePacketInjector();
#else
    std::cout << "Starting capturing ETH packets on device: " << ethInterface.getDeviceName() << std::endl;
    if (pcap_loop(NetworkDescriptor, CAPTURE_READ_TIMEOUT_MS, RecivePacketHandler, reinterpret_cast<uint8_t*>(this)) < 0) {
        throw NetworkException("Error capturing packets: " + std::string(pcap_geterr(NetworkDescriptor)));
    }
#endif
    return true;
}

void SimulatePacketInjector() {
        std::vector<std::vector<uint8_t>> SimulatedPackets;
        CreateSimulatedPackets(SimulatedPackets); 
        std::cout << "[MOCK-Capture] Injecting fake packets, packets to inject: " << SimulatedPackets.size() << std::endl << std::endl;
        if (SimulatedPackets.empty())
            std::cerr << "[ERROR-Capture] No packets created. Exiting injection loop." << std::endl;
        for (int PacketIndex = 0; PacketIndex < SimulatedPackets.size(); PacketIndex++) {
            std::cout << "[DEBUG-Capture] Packet number: " << PacketIndex <<". Current packet size: " << SimulatedPackets[PacketIndex].size() << std::endl;
            if (SimulatedPackets[PacketIndex].empty()) {
                std::cerr << "[DEBUG-Capture] Skipping empty payload." << std::endl;
            }
            if(PacketCapture::ReciveQueue.isFull()){
                std::cout << "[DEBUG-Capture] ReciveQueue is full" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            std::cout << "[DEBUG-Capture] Packets in queue: " << PacketCapture::ReciveQueue.getCurrentPacketsCount() << std::endl;
            if (PacketCapture::ReciveQueue.enqueue(SimulatedPackets[PacketIndex].data(), SimulatedPackets[PacketIndex].size())) {
                std::cout << "[MOCK-Capture] Simulated packet enqueued, length: " << SimulatedPackets[PacketIndex].size() << std::endl;
                std::cout << std::endl;
            } else {
                std::cerr << "[MOCK-Capture] Failed to enqueue simulated packet. Queue might be full." << std::endl;
            }
        }
        std::cout << "[DEBUG-Capture] Finished iterating through SimulatedPackets." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void CreateSimulatedPackets(std::vector<std::vector<uint8_t>>& packets) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dis(0, 255);
    packets.resize(PACKET_TO_CREATE, std::vector<uint8_t>(PACKET_SIZE));
    for (size_t i = 0; i < PACKET_TO_CREATE; i++) {
        std::string basePattern = "COMPRESSIBLE"; 
        size_t patternSize = basePattern.size();
        for (size_t j = 0; j < PACKET_SIZE; j++) {
            if (j % 4 == 0) {  
                packets[i][j] = basePattern[j % patternSize];  
            } else {
                packets[i][j] = dis(gen);
            }
        }
        std::cout << "[MOCK] Packet " << i + 1 << " created with semi-random compressible data.\n";
    }
}

/* Store the packets in the Queue without udp header, just raw data (payload) */
void PacketCapture::RecivePacketHandler(uint8_t* GlobalData, const struct pcap_pkthdr* PacketHeader, const uint8_t* PacketData) {
#ifndef MOCK_UP
    PacketRecived = true;
    std::cout << "Packet captured with length: " << PacketHeader->len << std::endl;
    const uint8_t *PayloadData = PacketData + UDP_HEADER_LENGTH;
    const size_t PayloadLen = PacketHeader -> len - 8; 
    if (PacketCapture::ReciveQueue.enqueue(PayloadData, PayloadLen)) 
        std::cout << "Packet added to queue, length: " << PayloadLen << std::endl;
    else
        std::cerr << "Failed to enqueue packet, queue might be full." << std::endl;
#endif
}

