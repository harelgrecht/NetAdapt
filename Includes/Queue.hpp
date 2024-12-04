#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../Includes/Global_Defines.hpp"


#include <vector>
#include <mutex>
#include <cstdint>
#include <algorithm>

class Queue {
private:
    std::vector<std::vector<uint8_t>> Packets; // using vector of vectors for storing packets
    size_t PacketSizes[MAX_QUEUE_CAPACITY]; 
    int Front;               
    int Rear;                 
    int CurrentlyPacketsCount;
    std::mutex Mutex;       
public:
    Queue();                
    bool enqueue(const uint8_t* data, size_t size);
    bool dequeue(uint8_t* data, size_t& size);      
    bool isEmpty();                 
    bool isFull();
    int PacketsCount();            
};

#endif // QUEUE_HPP
