#ifndef GLOBAL_DEFINES_HPP
#define GLOBAL_DEFINES_HPP

#include <string>

//GPIO_Handler Module
class GPIO;
#define LINK_STATUS_LED_PIN 506
#define PACKET_TRAFFIC_LED_PIN 507
#define GPIO_ON 1
#define GPIO_OFF 0
#define GPIO_OUT "OUT"
#define GPIO_IN "in"
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define GPIO_UNEXPORT_PATH "/sys/class/gpio/unexport"
#define GPIO_DIRECTION_PATH "/sys/class/gpio/gpio%d/direction"
#define GPIO_VALUE_PATH "/sys/class/gpio/gpio%d/value"
#define LED_BLINK_DURATION_MS 250
extern GPIO LinkStatusLed;
extern GPIO PacketTrafficStatusLed;

//ETH_Status Module
#define NUM_ETHERNET_DEVICES 2
#define MAX_INTERFACE_NAME_LENGTH (IFNAMSIZ - 1)
extern bool PacketRecived;

//Packet_Capture Module
#define PACKET_SIZE 360
#define MAX_COMPRESSED_PACKET_SIZE (PACKET_SIZE * 5)
#define UDP_HEADER_LENGTH 8
#define PROMISC 1
#define CAPTURE_READ_TIMEOUT_MS 1000
#define PACKET_CAPTURE_LOOP_COUNT -1
#define ETH_FILTER "udp and src host 192.168.1.1"

//Packet_Process Module
#define COMPRESSION_OVERHEAD_PERCENT (10/100)
#define ZLIB_OVERHEAD_BYTES 12
#define NUM_PACKETS_TO_FETCH 5

// Packet_Send Module
#define DEFAULT_TTL_VALUE 255
#define IP_HEADER_LENGTH_UNITS 5
#define IP_VERSION 4

// Queue Module
#define MAX_QUEUE_CAPACITY 100


//MUST Module
#define SOURCE_DEVICE "eth0"
#define SOURCE_IP "192.168.1.1"
#define SOURCE_PORT 1234
#define DESTINATION_DEVICE "eth1"
#define DESTINATION_IP "192.168.1.2"
#define DESTINATION_PORT 1234

#endif // GLOBAL_DEFINES_HPP