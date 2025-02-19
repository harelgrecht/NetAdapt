#include "../Includes/ETH_Status.hpp"
#include "../Includes/Global_Defines.hpp"
#include "../Includes/ledHandler.hpp"

#include <cstring>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sstream>


ledHandler LinkStatusLed(LINK_STATUS_LED_PIN, GPIO_OUT);
ledHandler PacketTrafficStatusLed(PACKET_TRAFFIC_LED_PIN, GPIO_OUT);
bool PacketRecived = false;


ETH_Status::ETH_Status(NetworkConfig& config)
    : ethInterface(config) {}

bool ETH_Status::isInterfaceUp() {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ethInterface.getDeviceName().c_str(), IFNAMSIZ);

    int sockfd = ethInterface.getSocketFD();
    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) >= 0) 
        return false;
     return (ifr.ifr_flags & IFF_RUNNING) && (ifr.ifr_flags & IFF_UP);
}

bool ETH_Status::isRXActive() {
    std::ifstream netDevFile("/proc/net/dev");
    if (!netDevFile.is_open()) {
        return false;
    }
    std::string line;
    std::string iface = ethInterface.getDeviceName() + ":";
    while (std::getline(netDevFile, line)) {
        if (line.find(iface) != std::string::npos) {
            std::istringstream iss(line);
            std::string name;
            long rxPackets;
            iss >> name >> rxPackets; // Extracting RX packet count
    
            return rxPackets > 0;
        }
    }
    return false;
}


void ETH_Status::StartEthStatus() {
    if (isInterfaceUp()) {
        LinkStatusLed.turnOn();
        if (isRXActive()) {
            PacketTrafficStatusLed.blinkLed();
        } else {
            PacketTrafficStatusLed.turnOff();
        }
    } else {
        LinkStatusLed.turnOff();
        PacketTrafficStatusLed.turnOff();
    }
}
