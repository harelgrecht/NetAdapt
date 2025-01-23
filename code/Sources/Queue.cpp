#include "../Includes/Queue.hpp"
#include <iostream>

Queue::Queue()
    : Front(0), Rear(0), CurrentlyPacketsCount(0), Packets(MAX_QUEUE_CAPACITY), PacketSizes(MAX_QUEUE_CAPACITY) {

}

bool Queue::isFull() {
    return CurrentlyPacketsCount == MAX_QUEUE_CAPACITY;
}

bool Queue::isEmpty() {
    return CurrentlyPacketsCount == 0;
}

bool Queue::enqueue(const uint8_t* data, size_t size) {
    if (size > PACKET_SIZE) return false;

    std::lock_guard<std::mutex> lock(Mutex);
    if (isFull()) return false;

    Packets[Rear] = std::vector<uint8_t>(data, data + size);
    PacketSizes[Rear] = size;
    Rear = (Rear + 1) % MAX_QUEUE_CAPACITY;
    CurrentlyPacketsCount++;
    return true;
}

bool Queue::dequeue(uint8_t* data, size_t& size) {
    std::lock_guard<std::mutex> lock(Mutex);
    if (isEmpty()) return false;

    size = PacketSizes[Front];
    std::copy(Packets[Front].begin(), Packets[Front].end(), data);
    Packets[Front].clear();
    Front = (Front + 1) % MAX_QUEUE_CAPACITY;
    CurrentlyPacketsCount--;
    return true;
}

int Queue::getCurrentPacketsCount() {
    std::lock_guard<std::mutex> lock(Mutex);
    return CurrentlyPacketsCount;

}
