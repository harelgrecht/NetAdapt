#include "../MUST.HPP"

Queue::Queue() : front(0), rear(0), count(0) {};

bool Queue::isFull() const {
    return count == QUEUE_SIZE;
}


bool Queue::isEmpty() const {
    return count == 0;
}

bool Queue::enqueue(const uint8_t* data, size_t size) {
    if (isFull() || size > PACKET_SIZE) return false;
    mutex.lock();
    //std::lock_guard<std::mutex> lock(mutex); //automaticlly locking and unlocking the mutex
    std::memcpy(packets[rear], data, size);
    sizes[rear] = size;
    rear = (rear + 1) % QUEUE_SIZE;
    count++;
    return true;
    mutex.unlock();
}

bool Queue::dequeue(uint8_t* data, size_t& size) {
    if (isEmpty()) return false;
    mutex.lock();
   // std::lock_guard<std::mutex> lock(mutex); //automaticlly locking and unlocking the mutex
    size = sizes[front];
    std::memcpy(data, packets[front], size);
    front = (front + 1) % QUEUE_SIZE;
    count --;
    return true;
    mutex.unlock();
}

int Queue::packetsInQueue() const {
    return this -> count;
}
