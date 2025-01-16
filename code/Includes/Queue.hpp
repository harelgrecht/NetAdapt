#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../Includes/Global_Defines.hpp"

#include <vector>
#include <mutex>
#include <cstdint>
#include <algorithm>

class Queue {
private:
    int Front;
    int Rear;
    int CurrentlyPacketsCount;
    std::vector<std::vector<uint8_t>> Packets;
    std::vector<size_t> PacketSizes;
    std::mutex Mutex;
 
public:
    Queue();                
    bool enqueue(const uint8_t* data, size_t size);
    bool dequeue(uint8_t* data, size_t& size);      
    bool isEmpty();                 
    bool isFull();
    int getCurrentPacketsCount();            
};

#endif // QUEUE_HPP