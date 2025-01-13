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
    while (true) {
        std::vector<std::vector<uint8_t>> SimulatedPackets;
        CreateSimulatedPackets(SimulatedPackets); // Create a batch of packets

        std::cout << "[MOCK] Injecting fake packets" << std::endl;
        std::cout << "[DEBUG] SimulatedPackets.size(): " << SimulatedPackets.size() << std::endl;

        if (SimulatedPackets.empty()) {
            std::cerr << "[ERROR] No packets created. Exiting injection loop." << std::endl;
            break; // Exit if no packets were created
        }

        for (const auto& payload : SimulatedPackets) {
            std::cout << "[DEBUG] Looping through payload. Current payload size: " << payload.size() << std::endl;

            if (payload.empty()) {
                std::cerr << "[DEBUG] Skipping empty payload." << std::endl;
                continue; // Skip empty payloads
            }

            std::cout << "[DEBUG] Payload size: " << payload.size() << std::endl;
            std::cout << "[DEBUG] Is queue full: " << PacketCapture::ReciveQueue.isFull() << std::endl;

            if (PacketCapture::ReciveQueue.enqueue(payload.data(), payload.size())) {
                std::cout << "[MOCK] Simulated packet enqueued, length: " << payload.size() << std::endl;
            } else {
                std::cerr << "[MOCK] Failed to enqueue simulated packet. Queue might be full." << std::endl;
            }
        }

        std::cout << "[DEBUG] Finished iterating through SimulatedPackets." << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Delay before every packet
    }
}


void CreateSimulatedPackets(std::vector<std::vector<uint8_t>>& packets) {
    std::cout << "[MOCK] Start create fake UDP pakcets" << std::endl;
    const std::string sourceIP = "192.168.1.2";
    const std::string destinationIP = "192.168.1.3";
    const size_t payloadSize = 360;

    // Random number generator setup
    std::random_device rd; // Seed for randomness
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<uint8_t> dis(0, 255); // Random byte distribution

    for (int i = 0; i < 15; i++) { // Loop condition fixed: i < 15
        std::vector<uint8_t> payload;
        payload.reserve(payloadSize); // Allocate enough memory
        for (size_t j = 0; j < payloadSize; j++) {
            payload.push_back(dis(gen));
        }
        packets.push_back(payload); // Add the generated packet to the list
        std::cout << "[MOCK] Packet " << i + 1 << " created.\n";
    }
    std::cout << "[MOCK] Finished creating packets." << std::endl;
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

