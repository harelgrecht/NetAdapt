#include "Queue.hpp"

Queue::Queue() : front(0), rear(0), count(0) {};

bool Queue::isFull() const {
    return count == QUEUE_SIZE;
}


bool Queue::isEmpty() const {
    return count == 0;
}

bool Queue::enqueue(const uint8_t* data, size_t size) {
    if (isFull() || size > PACKET_SIZE) return false;
    
    std::memcpy(packets[rear], data, size);
    sizes[rear] = size;
    rear = (rear + 1) % QUEUE_SIZE;
    count++;
    return true;
}

bool Queue::dequeue(uint8_t* data, size_t& size) {
    if (isEmpty()) return false;
    size = sizes[front];
    std::memcpy(packets[front], data, size);
    front = (front + 1) % QUEUE_SIZE;
    count --;
    return true;
}

int Queue::packetsInQueue() const {
    return this -> count;
}
