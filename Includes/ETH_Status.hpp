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

        
        static const std::string InterfaceMap[ETH_DEVICE_COUNT];
        uint32_t PreviousRxBytes[ETH_DEVICE_COUNT];
};

#endif