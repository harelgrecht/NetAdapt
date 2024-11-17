#include "Packet_Capture.hpp"

PacketCapture::PacketCapture(const std::string& device, std::string IpAddress) : Device(device), IpAddress(IpAddress) {
    SetIPAddress(Device,IpAddress);
}

PacketCapture::~PacketCapture() {
    if (Handle != nullptr)
        pcap_close(Handle);
}

bool PacketCapture::SetIPAddress(const std::string& device, const std::string& IpAddress) {
    std::string command = "sudo ifconfig " + device + " " + IpAddress;
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Failed to set ip addres on device " << device << std::endl;
        return false;
    }
    std::cout << "IP config succsefully" << std::endl;
    return true;
}


bool PacketCapture::OpenDevice() {
    Handle = pcap_open_live(Device.c_str(), MAX_PACKET_SIZE, PROMISC, READ_TIMOUT, ErrBuffer);
    if (Handle == nullptr){
        std::cerr << "Error opening Device" << ErrBuffer << std::endl;
        return false;
    }
    return true;
}

bool PacketCapture::StartCapture() {
    if  (OpenDevice() == false) {
        return false;
    }

    std::cout << "Starting capturing ETH packets on device " << Device << std::endl;
    if(pcap_loop(Handle, LOOP_COUNT, packetHandler, reinterpret_cast<u_char*>(this)) < 0) {
        std::cerr << "Error capturing packets: " << pcap_geterr(Handle) << std::endl;
        return false;
    }
    return true;
}

void PacketCapture::packetHandler(u_char* GlobalData, const struct pcap_pkthdr* PacketHeader, const u_char* PacketData) {
    if(ReciveQueue.enqueue(PacketData, PacketHeader -> len) == true) 
        std::cout << "Packet in legnth: " << PacketHeader -> len << "Sent to the queue" << std::endl;
    else
        std::cout << "Failed to send the packet to the queue" << std::endl;
}

