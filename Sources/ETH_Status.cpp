#include "../Includes/Global_Defines.hpp"
#include "../Includes/ETH_Status.hpp"
#include "../Includes/GPIO_Handler.hpp"

#include <cstring>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>


GPIO LinkStatusLed(LINK_STATUS_LED_PIN, GPIO_OUT);
GPIO PacketTrafficStatusLed(PACKET_TRAFFIC_LED_PIN, GPIO_OUT);
bool PacketRecived = false;

const std::string ETHStatus::InterfaceMap[NUM_ETHERNET_DEVICES] = {"eth0", "eth1"};


ETHStatus::ETHStatus() {};

void ETHStatus::StartEthStatus() {
    UpdateEthStatus();
}

void ETHStatus::UpdateEthStatus() {
    for(int i = 0; i < NUM_ETHERNET_DEVICES; i++){
        if (IsEthDeviceRunning(InterfaceMap[i]) == true) {
            LinkStatusLed.SetValue(GPIO_ON);
                if (PacketRecived == true) {
                    PacketTrafficStatusLed.BlinkLed();
                    PacketRecived = false;
                } else {
                    PacketTrafficStatusLed.SetValue(GPIO_OFF);
                }
        } else {
            LinkStatusLed.SetValue(GPIO_OFF);
        }
    }
}


bool ETHStatus::IsEthDeviceRunning(const std::string& DeviceName) {
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd < 0){
        perror("socket");
        return false;
    }
    struct ifreq ifr = {};
    strncpy(ifr.ifr_name, DeviceName.c_str(), MAX_INTERFACE_NAME_LENGTH);
    if(ioctl(socketfd, SIOCGIFFLAGS, &ifr) < 0) {
        perror("ioctl");
        close(socketfd);
        return false;
    }
    
    close(socketfd);
    return (ifr.ifr_flags & IFF_UP) && (ifr.ifr_flags & IFF_RUNNING);
    
}