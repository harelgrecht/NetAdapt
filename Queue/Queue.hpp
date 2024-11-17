#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstdint> 
#include <cstddef> 
#include <iostream>
#include <cstring>

constexpr int QUEUE_SIZE = 10;
constexpr int PACKET_SIZE = 360; 

class Queue {
private:
    uint8_t packets[QUEUE_SIZE][PACKET_SIZE]; 
    size_t sizes[QUEUE_SIZE]; 
    int front;               
    int rear;                 
    int count;               
public:
    Queue();                
    bool enqueue(const uint8_t* data, size_t size);
    bool dequeue(uint8_t* data, size_t& size);      
    bool isEmpty() const;                 
    bool isFull() const;
    int packetsInQueue() const;            
};

#endif // QUEUE_HPP