#include "../Includes/Libraries.hpp"

void PacketProcess::GetPackets() {
    RawDataBuffer.clear();
    uint8_t Packet[PACKET_SIZE];
    size_t PacketSize;
    for(int PacketIndex = 0; PacketIndex < PACKETS_TO_FETCH; PacketIndex++) {
        if(!PacketCapture::ReciveQueue.dequeue(Packet, PacketSize)) {
            std::cerr << "Failed to fetch packet" << std::endl;
            continue;
        }
        RawDataBuffer.insert(RawDataBuffer.end(), Packet, Packet + PacketSize);
    }
}

void PacketProcess::CompressPacket() {
    if (RawDataBuffer.empty()) 
        std::cerr << "BeforeCompressSize is empty" << std::endl;
    CompressedDataBuffer.resize(COMPRESSED_SIZE);
    uLongf CompressedSize = COMPRESSED_SIZE;
    int result = compress2(CompressedDataBuffer.data(), &CompressedSize, RawDataBuffer.data(), RawDataBuffer.size(), Z_BEST_COMPRESSION);
    if (result == Z_OK) {
        std::cout << "Compressed len is: " << CompressedSize << std::endl;
        CompressedDataBuffer.resize(CompressedSize);
    }
}

void PacketProcess::StorePackets() {
    int TotalSize = CompressedDataBuffer.size();
    int offset = 0;
    while (offset < TotalSize) {
        size_t Chunk = std::min(COMPRESSED_SIZE, TotalSize - offset);
        if (!SendQueue.enqueue(CompressedDataBuffer.data() + offset, Chunk))
            std::cerr << "Send Queue is full" << std::endl;
        offset += Chunk;
    }

}

void PacketProcess::ProcessStart() {
    GetPackets();
    CompressPacket();
    StorePackets();
}