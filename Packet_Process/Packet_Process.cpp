#include "../MUST.hpp"


void PacketProcess::GetPackets() {
    RawDataBuffer.clear();
    uint8_t Packet[MAX_PACKET_SIZE];
    size_t PacketSize;
    if (!PacketCapture::ReciveQueue.dequeue(Packet,PacketSize) || PacketSize != MAX_PACKET_SIZE) 
        std::cerr << "Failed to fetch packet" << std::endl;
    RawDataBuffer.insert(RawDataBuffer.end(), Packet, Packet + PacketSize);
}

void PacketProcess::CompressPacket() {
    if (RawDataBuffer.empty()) 
        std::cerr << "BeforeCompressSize is empty" << std::endl;
    CompressedDataBuffer.resize(COMPRESSED_SIZE);
    uLongf CompressedSize = COMPRESSED_SIZE;
    int result = compress2(CompressedDataBuffer.data(), &CompressedSize, RawDataBuffer.data(), RawDataBuffer.size(), Z_BEST_COMPRESSION);
    if (result == Z_OK) {
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