#include "../Includes/Global_Defines.hpp"
#include "../Includes/Packet_Capture.hpp"
#include "../Includes/Exceptions.hpp"

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

PacketCapture::PacketCapture(const std::string& device, std::string IpAddress) : Device(device), IpAddress(IpAddress) {
    SetIPAddress(Device,IpAddress);
}

PacketCapture::~PacketCapture() {
    if (NetworkDescriptor != nullptr)
        pcap_close(NetworkDescriptor);
}

bool PacketCapture::SetIPAddress(const std::string& device, const std::string& IpAddress) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        throw NetworkException("Failed to create a socket for setting IP address.");
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, device.c_str(), MAX_INTERFACE_NAME_LENGTH);
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = 0;

    if (inet_pton(AF_INET, IpAddress.c_str(), &addr.sin_addr) != 1) {
        std::cerr << "Invalid ip address format: " << IpAddress << std::endl;
        close(sockfd);
        throw NetworkException("Invalid IP address format: " + IpAddress);
    }

    memcpy(&ifr.ifr_addr, &addr, sizeof(struct sockaddr_in));

    if(ioctl(sockfd, SIOCSIFADDR, &ifr) <0 ) {
        std::cerr << "Failed to set IP address on device: " << device << std::endl;
        close(sockfd);
        throw NetworkException("Failed to set IP address on device: " + device);
    }
    std::cout << "IP address successfully set on device: " << device << std::endl;
    close(sockfd);
    return true;
}   

bool PacketCapture::OpenDevice() {
    NetworkDescriptor = pcap_open_live(Device.c_str(), PACKET_SIZE, PROMISC, CAPTURE_READ_TIMEOUT_MS, ErrBuffer);
    if (NetworkDescriptor == nullptr){
        throw NetworkException("Error opening device: " + std::string(ErrBuffer));
    }
    return true;
}

bool PacketCapture::SetFilter(const std::string& FilterString) {
    if (!NetworkDescriptor) 
        throw NetworkException("Device is not open.");
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
    return true;
}

bool PacketCapture::StartCapture(const std::string& FilterString) {
    try {
        OpenDevice();
        SetFilter(FilterString);
    } catch (const NetworkException& e) {
        std::cerr << "Error starting capture: " << e.what() << std::endl;
    }

#ifdef MOCK_UP
    SimulatePacketInjector();
#else
    std::cout << "Starting capturing ETH packets on device " << Device << std::endl;
    if (pcap_loop(NetworkDescriptor, CAPTURE_READ_TIMEOUT_MS, RecivePacketHandler, reinterpret_cast<uint8_t*>(this)) < 0) {
        throw NetworkException("Error capturing packets: " + std::string(pcap_geterr(NetworkDescriptor)));
    }
#endif
    return true;
}

void SimulatePacketInjector() {
    while(true){
        std::vector<std::vector<uint8_t>> SimulatedPackets;
        CreateSimulatedPackets(SimulatedPackets); //Create batch of packets

        for (const auto& payload : SimulatedPackets) { // Ensures enqueing each packet individually from the batch created before
        if (PacketCapture::ReciveQueue.enqueue(payload.data(), payload.size())) 
            std::cout << "[MOCK] Simulated packet enqueued, length: " << payload.size() << std::endl;
        else 
         std::cerr << "[MOCK] Failed to enqueue simulated packet. Queue might be full." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); //Delay before every packet
    }
}

void CreateSimulatedPackets(std::vector<std::vector<uint8_t>>& packets) {
    const std::string sourceIP = "192.168.1.2";
    const std::string destinationIP = "192.168.1.3";
    const size_t payloadSize = 360;

    // Random number generator setup
    std::random_device rd; // Seed for randomness
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<uint8_t> dis(0, 255); // Random byte distribution

    for (int i = 0; i > 15; i++) { //excpeting to see 3 compressed packets at the end
        std::vector<uint8_t> payload;
        payload.reserve(payloadSize); //Alocate enough memory
        for (size_t j = 0; j < payloadSize; j++)
            payload.push_back(dis(gen));
        packets.push_back(payload);
    }
}

/* Store the packets in the Queue without udp header, just raw data (payload) */
void PacketCapture::RecivePacketHandler(uint8_t* GlobalData, const struct pcap_pkthdr* PacketHeader, const uint8_t* PacketData) {
    PacketRecived = true;
    std::cout << "Packet captured with length: " << PacketHeader->len << std::endl;
    const uint8_t *PayloadData = PacketData + UDP_HEADER_LENGTH;
    const size_t PayloadLen = PacketHeader -> len - 8; 
    if (ReciveQueue.enqueue(PayloadData, PayloadLen)) 
        std::cout << "Packet added to queue, length: " << PayloadLen << std::endl;
    else
        std::cerr << "Failed to enqueue packet, queue might be full." << std::endl;

}

