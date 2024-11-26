#include "../MUST.HPP"

PacketCapture::PacketCapture(const std::string& device, std::string IpAddress) : Device(device), IpAddress(IpAddress) {
    SetIPAddress(Device,IpAddress);
}

PacketCapture::~PacketCapture() {
    if (Handle != nullptr)
        pcap_close(Handle);
}

bool PacketCapture::SetIPAddress(const std::string& device, const std::string& IpAddress) {
    //TODO: change that so it wont use the system command
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

bool PacketCapture::SetFilter(const std::string& FilterString) {
    if (!Handle) {
        std::cerr << "Device is not open" << std::endl;
        return false;
    }
    struct bpf_program filter;
    if(pcap_compile(Handle, &filter, FilterString.c_str(),0, PCAP_NETMASK_UNKNOWN) == -1) {
        std::cerr << "Failed to compile filter: " << pcap_geterr(Handle) << std::endl;
        pcap_freecode(&filter);
        return false; 
    }
    pcap_freecode(&filter);
    std::cout << "Filter applied: " << FilterString << std::endl;
    return true;
}

bool PacketCapture::StartCapture(const std::string& FilterString) {
    if  (OpenDevice() == false) {
        return false;
    }
    if (!SetFilter(FilterString)) {
        return false;
    }
    std::cout << "Starting capturing ETH packets on device " << Device << std::endl;
    if(pcap_loop(Handle, LOOP_COUNT, RecivePacketHandler, reinterpret_cast<uint8_t*>(this)) < 0) {
        std::cerr << "Error capturing packets: " << pcap_geterr(Handle) << std::endl;
        return false;
    }
    return true;
}

/* Store the packets in the Queue without udp header, just raw data (payload) */
void PacketCapture::RecivePacketHandler(uint8_t* GlobalData, const struct pcap_pkthdr* PacketHeader, const uint8_t* PacketData) {
    const uint8_t *PayloadData = PacketData + UDP_HEADER_SIZE;
    const size_t PayloadLen = PacketHeader -> len - 8; 
    if(ReciveQueue.enqueue(PayloadData, PayloadLen) == true) 
        std::cout << "Packet in legnth: " << PacketHeader -> len << "Sent to the queue" << std::endl;
    else
        std::cout << "Failed to send the packet to the queue" << std::endl;
}

