#pragma once
#ifndef ETH_STATUS_HPP
#define ETH_STATUS_HPP

#include "Libraries.hpp"


class ETHStatus {
    public:
        ETHStatus();
        void StartEthStatus();
        
    private:
        void UpdateEthStatus();
        bool IsEthDeviceRunning(const std::string& DeviceName);

        
        static const std::string InterfaceMap[ETH_DEVICE_COUNT];
        uint32_t PreviousRxBytes[ETH_DEVICE_COUNT];
        GPIO LinkStatusLed;
        GPIO PacketTrafficStatusLed;
};



#endif