#include "GPIO_Handler/GPIO_Handler.hpp"
#include "Packet_Capture/Packet_Capture.hpp"
#include "Packet_Process/Packet_Process.hpp"
#include "Keep_Running/Keep_Running.hpp"
#include "Packet_Send/Packet_Send.hpp"
#include <thread>
#include <csignal>

#define ReciverIP "192.168.1.1"
#define ReciverDevice "eth0"

std::string filter = "udp and src host 192.168.1.1";