#include "../Includes/Queue.hpp"
#include "../Includes/Global_Defines.hpp"
#include <iostream>

Queue::Queue() : Packets(MAX_QUEUE_CAPACITY), CurrentlyPacketsCount(0), Front(0), Rear(0) {}

bool Queue::isFull() const {
    Mutex.lock();
    bool result = CurrentlyPacketsCount == MAX_QUEUE_CAPACITY;
    Mutex.unlock();
    return result;
}

bool Queue::isEmpty() const{
    Mutex.lock();
    bool result = CurrentlyPacketsCount == 0;
    Mutex.unlock();
    return result;
}

bool Queue::enqueue(const uint8_t* data, size_t size) {
    if (size > PACKET_SIZE) return false;
    Mutex.lock();
    if (isFull()) {
        Mutex.unlock();
        return false;
    }
    Packets[Rear] = std::vector<uint8_t>(data, data+ size);
    PacketSizes[Rear] = size; 
    Rear = (Rear + 1) % MAX_QUEUE_CAPACITY; 
    CurrentlyPacketsCount++;
    Mutex.unlock();
    return true;
}

bool Queue::dequeue(uint8_t* data, size_t& size) const{
    Mutex.lock();
    if (isEmpty()) {
        Mutex.unlock();
        return false;
    }
    size = PacketSizes[Front];
    std::copy(Packets[Front].begin(), Packets[Front].end(), data);
    Packets[Front].clear();
    Front = (Front + 1) % MAX_QUEUE_CAPACITY;
    CurrentlyPacketsCount--;
    Mutex.unlock();
    return true;
}

int Queue::getCurrentlyPacketsCount() const{
    Mutex.lock();
    return CurrentlyPacketsCount;
    Mutex.unlock();
}
