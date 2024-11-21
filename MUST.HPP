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

#include "GPIO_Handler/GPIO_Handler.hpp"
#include "Packet_Capture/Packet_Capture.hpp"
#include "Packet_Process/Packet_Process.hpp"
#include "Keep_Running/Keep_Running.hpp"
#include "Packet_Send/Packet_Send.hpp"
#include "Queue/Queue.hpp"


#define GPIO_ON 1
#define GPIO_OFF 0
#define GPIO_OUT "OUT"
#define GPIO_IN "in"

#define MAX_PACKET_SIZE 630
#define PROMISC 1
#define READ_TIMOUT 1000 // If no packets recived in 1000ms
#define LOOP_COUNT -1


#define ReciverIP "192.168.1.1"
#define ReciverDevice "eth0"
#define UDPHeaderSize 8
std::string filter = "udp and src host 192.168.1.1";

#define QUEUE_SIZE 10
#define PACKET_SIZE 360 