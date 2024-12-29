#ifndef ETH_STATUS_HPP
#define ETH_STATUS_HPP


#include <string>
#include <cstdint>
#include <array>

class ETHStatus {
    public:
        ETHStatus();
        void StartEthStatus();
        
    private:
        void UpdateEthStatus();
        bool IsEthDeviceRunning(const std::string& DeviceName);

        
        static const std::string InterfaceMap[NUM_ETHERNET_DEVICES];
};

#endif