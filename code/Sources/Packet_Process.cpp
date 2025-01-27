#include "../Includes/Packet_Process.hpp"
#include "../Includes/Packet_Capture.hpp"
#include "../Includes/Global_Defines.hpp"
#include "../Includes/Queue.hpp"

#include <iostream>
#include <algorithm>
#include <zlib.h>
#include <chrono>
#include <thread>


Queue PacketProcess::SendQueue;

void PacketProcess::GetPackets() {
    RawDataBuffer.clear();
    uint8_t Packet[PACKET_SIZE];
    size_t PacketSize;
    for (int i = 0; i < NUM_PACKETS_TO_FETCH; ++i) {
        if (!PacketCapture::ReciveQueue.dequeue(Packet, PacketSize)) {
            std::cerr << "[ERROR-Process] Queue empty. Retrying..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Retry delay
            continue;
        }
        RawDataBuffer.insert(RawDataBuffer.end(), Packet, Packet + PacketSize);
    }

    if (RawDataBuffer.empty()) {
        std::cerr << "[CRITICAL ERROR-Process] RawDataBuffer is empty after retries." << std::endl;
        return;
    }
}


void PacketProcess::CompressPacket() {
    if (RawDataBuffer.empty()) {
        std::cerr << "RawDataBuffer is empty" << std::endl;
        return;
    }  
    std::cout << "[DEBUG-Process] RawDataBuffer.size(): " << RawDataBuffer.size() << std::endl;
    CompressedDataBuffer.resize(RawDataBuffer.size() + (RawDataBuffer.size() * COMPRESSION_OVERHEAD_PERCENT) + ZLIB_OVERHEAD_BYTES); //  10% overhead + zlib extra space
    uLongf CompressedSize = CompressedDataBuffer.size();
    int result = compress2(CompressedDataBuffer.data(), &CompressedSize, RawDataBuffer.data(), RawDataBuffer.size(), Z_BEST_COMPRESSION);
    if (result == Z_OK) {
        std::cout << "Compressed len is: " << CompressedSize << std::endl;
        CompressedDataBuffer.resize(CompressedSize);
        return;
    }
}

//implement a decompress function and make a test if the decompress == compress 

void PacketProcess::StorePackets() {
    int TotalSize = CompressedDataBuffer.size();
    int offset = 0;

    while (offset < TotalSize) {
        size_t Chunk = std::min(MAX_COMPRESSED_PACKET_SIZE, TotalSize - offset);

        if (SendQueue.isFull()) {
            std::cerr << "[ERROR-Process] SendQueue is full. Cannot enqueue more packets." << std::endl;
            break; // Exit to avoid infinite loop
        }

        if (!SendQueue.enqueue(CompressedDataBuffer.data() + offset, Chunk)) {
            std::cerr << "[ERROR-Process] Failed to enqueue compressed packet." << std::endl;
            break; // Exit to avoid repeated enqueue failures
        }

        offset += Chunk;
    }

    if (offset < TotalSize) {
        std::cerr << "[ERROR-Process] Not all compressed data was enqueued." << std::endl;
    }
}


void PacketProcess::ProcessStart() {
    GetPackets();
    CompressPacket();
#ifdef MOCK_UP
    DecompressPacket();
#else
    StorePackets();
#endif
}