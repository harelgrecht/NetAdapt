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
        std::cerr << "Failed to create Socket" << std::endl;
}

void PacketSend::CloseSocket() {
    if (Socket >= 0)
        closesocket(Socket);
}

void PacketSend::SendPacket(const std::string& DestIP, const int& DestPort, const uint8_t& PayloadData, size_t PayloadLen) {
    sockaddr_in destAddr = {};
    destAddr.sin_family = AF_INET; //Declaring IPv4
    destAddr.sin_port = htons(DestPort); //Conver port to network byte order
    if (inet_pton(AF_INET, DestIP.c_str(), &destAddr.sin_addr) <= 0) //Convert the dest ip to binary
        std::cerr << "Invalid destionation IP address" << std::endl;


    ssize_t SentBytes = sendto(Socket,PayloadData.c_str(), PayloadLen, 0, reinterpret_cast<sockaddr*>(&destAddr), sizeof(destAddr));
    std::cout << "Packet sent to " << DestIP << ":" << DestPort << " (" << PayloadLen << " bytes)" << std::endl;}