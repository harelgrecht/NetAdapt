#include "../Includes/Global_Defines.hpp"
#include "../Includes/Packet_Capture.hpp"

#include <cstring>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <net/if.h>
#include <iostream>
#include <stdexcept>

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
    if (sockfd < 0){
        std::cerr << "Failed to create a socket for setting IP address" << std::endl;
        return false;
    }

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, device.c_str(), MAX_INTERFACE_NAME_LENGTH);
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = 0;

    if (inet_pton(AF_INET, ipAddress.c_str(), &addr.sin_addr) != 1) {
        std::cerr << "Invalid ip address format: " << IpAddress << std::endl;
        close(sockfd);
        return false;
    }

    memcpy(&ifr.ifr_addr, &addr, sizeof(struct sockaddr_in));

    if(ioctl(sockfd, SIOCSIFADDR, &ifr) <0 ) {
        std::cerr << "Failed to set IP address on device: " << device << std::endl;
        close(sockfd);
        return false;
    }
    std::cout << "IP address successfully set on device: " << device << std::endl;
    close(sockfd);
    return true;
}   

bool PacketCapture::OpenDevice() {
    NetworkDescriptor = pcap_open_live(Device.c_str(), PACKET_SIZE, PROMISC, CAPTURE_READ_TIMEOUT_MS, ErrBuffer);
    if (NetworkDescriptor == nullptr){
        std::cerr << "Error opening Device" << ErrBuffer << std::endl;
        return false;
    }
    return true;
}

bool PacketCapture::SetFilter(const std::string& FilterString) {
    if (!NetworkDescriptor) {
        std::cerr << "Device is not open" << std::endl;
        return false;
    }
    struct bpf_program bpf_filter;
    if(pcap_compile(NetworkDescriptor, &bpf_filter, FilterString.c_str(),0, PCAP_NETMASK_UNKNOWN) == -1) {
        std::cerr << "Failed to compile filter: " << pcap_geterr(NetworkDescriptor) << std::endl;
        pcap_freecode(&bpf_filter);
        return false; 
    }

    if (pcap_setfilter(NetworkDescriptor, &bpf_filter) == -1) {
        std::cerr << "Failed to set filter: " << pcap_geterr(NetworkDescriptor) << std::endl;
        pcap_freecode(&bpf_filter);
        return false;
    }
        
    pcap_freecode(&bpf_filter);
    std::cout << "Filter applied: " << FilterString << std::endl;
    return true;
}

bool PacketCapture::StartCapture(const std::string& FilterString) {
    if  (OpenDevice() == false) {
        return false;
    }
    if (SetFilter(FilterString) == false) {
        return false;
    }
    std::cout << "Starting capturing ETH packets on device " << Device << std::endl;
    if(pcap_loop(NetworkDescriptor, CAPTURE_READ_TIMEOUT_MS, RecivePacketHandler, reinterpret_cast<uint8_t*>(this)) < 0) {
        std::cerr << "Error capturing packets: " << pcap_geterr(NetworkDescriptor) << std::endl;
        return false;
    }
    return true;
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

