#include "../Includes/Libraries.hpp"

ETHStatus::ETHStatus() : LinkStatusLed(GPIO1, GPIO_OUT), PacketTrafficStatusLed(GPIO2, GPIO_OUT){
    std::fill(std::begin(PreviousRxBytes), std::end(PreviousRxBytes), 0);
};

void ETHStatus::StartEthStatus() {
    UpdateEthStatus();
}

void ETHStatus::UpdateEthStatus() {
    for(int i = 0; i < ETH_DEVICE_COUNT; i++){
        if (IsEthDeviceRunning(InterfaceMap[i]) == true) {
            LinkStatusLed.SetValue(GPIO_ON);
                if (PacketRecived == true) {
                    PacketTrafficStatusLed.BlinkLed();
                    PacketRecived = false;
                } else {
                    PacketTrafficStatusLed.SetValue(0);
                }
        } else {
            LinkStatusLed.SetValue(GPIO_OFF);
        }
    }
}


bool ETHStatus::IsEthDeviceRunning(const std::string& DeviceName) {
    int socketfd = socket(AF_INET, SOCK_DGRAM, SOCKET_PROTOCTOL);
    if (socketfd < 0){
        perror("socket");
        return false;
    }
    struct ifreq ifr;
    strncpy(ifr.ifr_name, DeviceName.c_str(), IFNAMSIZ - 1);
    ioctl(socketfd, SIOCGIFFLAGS, &ifr);

    close(socketfd);
    return (ifr.ifr_flags & IFF_UP) && (ifr.ifr_flags & IFF_RUNNING);
    
}