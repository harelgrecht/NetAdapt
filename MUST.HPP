#include <csignal>
#include <cstdint>
#include <cstddef>
#include <iostream>
#include <cstring>
#include <mutex>
#include <pcap.h>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <zlib.h>
#include <thread>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#include "GPIO_Handler/GPIO_Handler.hpp"
#include "Packet_Capture/Packet_Capture.hpp"
#include "Packet_Process/Packet_Process.hpp"
#include "Keep_Running/Keep_Running.hpp"
#include "Packet_Send/Packet_Send.hpp"
#include "Queue/Queue.hpp"

#define GPIO1 506
#define GPIO2 507

#define GPIO_ON 1
#define GPIO_OFF 0
#define GPIO_OUT "OUT"
#define GPIO_IN "in"

#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define GPIO_UNEXPORT_PATH "/sys/class/gpio/unexport"
#define GPIO_DIRECTION_PATH "/sys/class/gpio/gpio" + std::to_string(PinNumber) + "/direction"
#define GPIO_VALUE_PATH "/sys/class/gpio/gpio" + std::to_string(PinNumber) + "/value"

#define MAX_PACKET_SIZE 630
#define PROMISC 1
#define READ_TIMOUT 1000 // If no packets recived in 1000ms
#define LOOP_COUNT -1


#define RECIVER_IP "192.168.1.1"
#define RECIVER_DEVICE "eth0"
#define UDP_HEADER_SIZE 8
const std::string filter = "udp and src host 192.168.1.1"; // it should be a const so nothing could be touching it mid work

#define DESTINATION_IP "192.168.1.2"
#define DESTINATION_PORT 5353 //what ever
#define DESTINATION_DEVICE "eth1"

#define QUEUE_SIZE 10
#define PACKET_SIZE 360 

#define SOCKET_PROTOCTOL 0

#define SENDTO_FLAG 0