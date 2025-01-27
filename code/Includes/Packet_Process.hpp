#ifndef PACKET_PROCESS_HPP
#define PACKET_PROCESS_HPP

#include "Queue.hpp"

#include <vector>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <zlib.h>


class PacketProcess {
    private:
        std::vector<uint8_t> RawDataBuffer;
        std::vector<uint8_t> CompressedDataBuffer;

    public:
        static Queue SendQueue;
        
        void ProcessStart();
        void GetPackets(); // will get 5 packets from the queue and store them in a big buffer 630 * 5 ??
        void CompressPacket(); //Will take the big buffer and compress it to a small buffer of 360 if the big buffer will be compress to many small packets of 360 it will give them and id
        void StorePackets(); // will send the packets to another Queue
        void DecompressPacket();

};



#endif