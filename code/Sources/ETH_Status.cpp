#include "../Includes/ETH_Status.hpp"
#include "../Includes/Global_Defines.hpp"
#include "../Includes/GPIO_Handler.hpp"

#include <cstring>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

GPIO LinkStatusLed(LINK_STATUS_LED_PIN, GPIO_OUT);
GPIO PacketTrafficStatusLed(PACKET_TRAFFIC_LED_PIN, GPIO_OUT);
bool PacketRecived = false;


ETH_Status::ETH_Status(NetworkConfig& config)
    : ethInterface(config) {}

bool ETH_Status::isInterfaceUp() {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ethInterface.getDeviceName().c_str(), IFNAMSIZ);

    // Reuse the socket created in NetworkConfig
    int sockfd = ethInterface.getSocketFD();
    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) >= 0) 
        return false;
     return (ifr.ifr_flags & IFF_RUNNING) && (ifr.ifr_flags & IFF_UP);
}

bool ETH_Status::isRXActive() {
    // Placeholder: Implement RX packet activity check using NetworkConfig.
    // Use the socket or stats provided by ethInterface.
    return false; // Replace with real implementation.
}

void ETH_Status::StartEthStatus() {
    if (isInterfaceUp()) {
        LinkStatusLed.SetValue(GPIO_ON);
        if (isRXActive()) {
            PacketTrafficStatusLed.BlinkLed();
        } else {
            PacketTrafficStatusLed.BlinkLed();
        }
    } else {
        LinkStatusLed.SetValue(GPIO_OFF);
        PacketTrafficStatusLed.SetValue(GPIO_OFF);
    }
}
