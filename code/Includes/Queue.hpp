#pragma once

#include <vector>
#include <mutex>
#include <optional>
#include <cstdint>

class Queue {
public:
    Queue();

    bool isFull() const;
    bool isEmpty() const;
    bool enqueue(const uint8_t* data, size_t size);
    std::optional<std::vector<uint8_t>> dequeue();
    int getCurrentlyPacketsCount() const;

private:
    mutable std::mutex Mutex;                 
    std::vector<std::vector<uint8_t>> Packets;
    std::vector<size_t> PacketSizes;          
    size_t CurrentlyPacketsCount = 0;         
    size_t Front = 0;                         
    size_t Rear = 0;                          
};
