#ifndef PACKET_PROCESS_HPP
#define PACKET_PROCESS_HPP

#include <iostream>
#include <zlib.h>
#include <vector>
#include <cstring>
#include <algorithm>
#include "../Queue/Queue.hpp"
#include "../Packet_Capture/Packet_Capture.hpp"

#define PACKET_SIZE 630
#define BUFFER_SIZE PACKET_SIZE * 5
#define COMPRESSED_SIZE 360

class PacketProcess {
    protected:
        static Queue SendQueue;

    private:
        std::vector<uint8_t> BeforeCompressBuffer;
        std::vector<uint8_t> AfterCompressBuffer;

    public:
        void GetPackets(); // will get 5 packets from the queue and store them in a big buffer 630 * 5 ??
        void CompressPacket(); //Will take the big buffer and compress it to a small buffer of 360 if the big buffer will be compress to many small packets of 360 it will give them and id
        void StorePackets(); // will send the packets to another Queue
};



#endif