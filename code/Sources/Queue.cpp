#include "../Includes/Queue.hpp"
#include <iostream>

Queue::Queue()
    : Front(0), Rear(0), CurrentlyPacketsCount(0), Packets(MAX_QUEUE_CAPACITY), PacketSizes(MAX_QUEUE_CAPACITY) {
    std::cout << "[DEBUG] Queue initialized. Address: " << this << "\n";
}

bool Queue::isFull() {
#ifdef MOCK_UP
    std::cout << "[MOCK] Checking if queue is full. CurrentlyPacketsCount: "
              << CurrentlyPacketsCount << ", Max Capacity: " << MAX_QUEUE_CAPACITY << "\n";
    return CurrentlyPacketsCount == MAX_QUEUE_CAPACITY;
#else
    std::lock_guard<std::mutex> lock(Mutex);
    return CurrentlyPacketsCount == MAX_QUEUE_CAPACITY;
#endif
}

bool Queue::isEmpty() {
#ifdef MOCK_UP
    std::cout << "[MOCK] Checking if queue is empty. CurrentlyPacketsCount: "
              << CurrentlyPacketsCount << "\n";
    return CurrentlyPacketsCount == 0;
#else
    std::lock_guard<std::mutex> lock(Mutex);
    return CurrentlyPacketsCount == 0;
#endif
}

bool Queue::enqueue(const uint8_t* data, size_t size) {
    if (size > PACKET_SIZE) return false;

#ifdef MOCK_UP
    if (isFull()) {
        std::cout << "[MOCK] Queue is full, cannot enqueue.\n";
        return false;
    }
#else
    std::lock_guard<std::mutex> lock(Mutex);
    if (isFull()) return false;
#endif

    Packets[Rear] = std::vector<uint8_t>(data, data + size);
    PacketSizes[Rear] = size;
    Rear = (Rear + 1) % MAX_QUEUE_CAPACITY;
    CurrentlyPacketsCount++;
    return true;
}

bool Queue::dequeue(uint8_t* data, size_t& size) {
#ifdef MOCK_UP
    if (isEmpty()) {
        std::cout << "[MOCK] Queue is empty, cannot dequeue.\n";
        return false;
    }
#else
    std::lock_guard<std::mutex> lock(Mutex);
    if (isEmpty()) return false;
#endif

    size = PacketSizes[Front];
    std::copy(Packets[Front].begin(), Packets[Front].end(), data);
    Packets[Front].clear();
    Front = (Front + 1) % MAX_QUEUE_CAPACITY;
    CurrentlyPacketsCount--;
    return true;
}

int Queue::getCurrentPacketsCount() {
#ifdef MOCK_UP
    std::cout << "[MOCK] Getting current packet count: " << CurrentlyPacketsCount << "\n";
    return CurrentlyPacketsCount;
#else
    std::lock_guard<std::mutex> lock(Mutex);
    return CurrentlyPacketsCount;
#endif
}
