#include "../Includes/Libraries.hpp"

Queue::Queue() : Front(0), Rear(0), CurrentlyPacketsCount(0), Packets(QUEUE_SIZE) {};

bool Queue::isFull() {
    Mutex.lock();
    bool result = CurrentlyPacketsCount == QUEUE_SIZE;
    Mutex.unlock();
    return result;
}

bool Queue::isEmpty() {
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
    Rear = (Rear + 1) % QUEUE_SIZE; 
    CurrentlyPacketsCount++;
    return true;
    Mutex.unlock();
}

bool Queue::dequeue(uint8_t* data, size_t& size) {
    Mutex.lock();
    if (isEmpty()) {
        Mutex.unlock();
        return false;
    }
    size = PacketSizes[Front];
    std::copy(Packets[Front].begin(), Packets[Front].end(), data);
    Packets[Front].clear();
    Front = (Front + 1) % QUEUE_SIZE;
    CurrentlyPacketsCount--;
    return true;
    Mutex.unlock();
}

int Queue::PacketsCount() {
    Mutex.lock();
    int PacketCount = CurrentlyPacketsCount;
    Mutex.unlock();
    return PacketCount;
}
