#include "Packet_Process.hpp"


void PacketProcess::GetPackets() {
    BeforeCompressBuffer.clear();
    uint8_t Packet[MAX_PACKET_SIZE];
    size_t PacketSize;
    if (!PacketCapture::ReciveQueue.dequeue(Packet,PacketSize) || PacketSize != MAX_PACKET_SIZE) 
        std::cerr << "Failed to fetch packet" << std::endl;
    BeforeCompressBuffer.insert(BeforeCompressBuffer.end(), Packet, Packet + PacketSize);
}

void PacketProcess::CompressPacket() {
    if (BeforeCompressBuffer.empty()) 
        std::cerr << "BeforeCompressSize is empty" << std::endl;
    AfterCompressBuffer.resize(COMPRESSED_SIZE);
    uLongf CompressedSize = COMPRESSED_SIZE;
    int result = compress2(AfterCompressBuffer.data(), &CompressedSize, BeforeCompressBuffer.data(), BeforeCompressBuffer.size(), Z_BEST_COMPRESSION);
    if (result == Z_OK) {
        AfterCompressBuffer.resize(CompressedSize);
    }
}

void PacketProcess::StorePackets() {
    int TotalSize = AfterCompressBuffer.size();
    int offset = 0;
    while (offset < TotalSize) {
        size_t Chunk = std::min(COMPRESSED_SIZE, TotalSize - offset);
        if (!SendQueue.enqueue(AfterCompressBuffer.data() + offset, Chunk))
            std::cerr << "Send Queue is full" << std::endl;
        offset += Chunk;
    }

}