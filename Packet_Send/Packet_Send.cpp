#include "../MUST.HPP"

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
        closesocket(Socket);
}

void PacketSend::GetPackets() {
    if(PacketProcess::SendQueue.dequeue(Payload, PayloadLen) == false) {
        std::cout << "Failed to get a packet from the SendQueue" << std::endl;
    }
}

void PacketSend::SendPacket(const std::string& DestIP, const int& DestPort) {
    GetPackets();
    sockaddr_in destAddr = {};
    destAddr.sin_family = AF_INET; //Declaring IPv4
    destAddr.sin_port = htons(DestPort); //Conver port to network byte order
    if (inet_pton(AF_INET, DestIP.c_str(), &destAddr.sin_addr) <= 0) //Convert the dest ip to binary
        std::cerr << "Invalid destionation IP address" << std::endl;
    ssize_t SentBytes = sendto(Socket,PayloadData.c_str(), PayloadLen, SENDTO_FLAG, reinterpret_cast<sockaddr*>(&destAddr), sizeof(destAddr));
    std::cout << "Packet sent to " << DestIP << ":" << DestPort << " (" << PayloadLen << " bytes)" << std::endl;}