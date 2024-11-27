#include "../Includes/Libraries.hpp"

PacketSend::PacketSend() {
    CreateSocket();
};

PacketSend::~PacketSend() {
    CloseSocket();
}

void PacketSend::CreateSocket() {
    Socket = socket(AF_INET, SOCK_DGRAM, SOCKET_PROTOCTOL);
    if (Socket < 0)
        throw std::runtime_error("Failed to create socket: " + std::string(std::strerror(errno)));
}

void PacketSend::CloseSocket() {
    if (Socket >= 0)
        close(Socket);
}

void PacketSend::GetPackets() {
    if(PacketProcess::SendQueue.dequeue(PayloadData, PayloadLen) == false) {
        std::cout << "Failed to get a packet from the SendQueue" << std::endl;
    }
}

void PacketSend::BindSocketToInterface(const std::string& DeviceName) {
    struct ifreq ifr;
    std::memset(&ifr, 0, sizeof(ifr));
    std::strncpy(ifr.ifr_name, DeviceName.c_str(), IFNAMSIZ - 1);

    // Bind the socket to the specified network interface
    if (setsockopt(Socket, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof(ifr)) < 0) {
        std::cerr << "Failed to bind socket to interface: " << std::strerror(errno) << std::endl;
    }
}

void PacketSend::SendPacket(const std::string& DestIP, const int& DestPort, const std::string& destDevice) {
    GetPackets();
    if(!destDevice.empty())
        BindSocketToInterface(destDevice);
    struct sockaddr_in destAddr = {};
    std::memset(&destAddr,0, sizeof(destAddr));
    destAddr.sin_family = AF_INET; //Declaring IPv4
    destAddr.sin_port = htons(DestPort); //Conver port to network byte order
    if (inet_pton(AF_INET, DestIP.c_str(), &destAddr.sin_addr) <= 0) //Convert the dest ip to binary
        std::cerr << "Invalid destionation IP address" << std::endl;
    ssize_t SentBytes = sendto(Socket, PayloadData, PayloadLen, SENDTO_FLAG, reinterpret_cast<sockaddr*>(&destAddr), sizeof(destAddr));
    std::cout << "Packet sent to " << DestIP << ":" << DestPort << " (" << SentBytes << " bytes)" << std::endl;}