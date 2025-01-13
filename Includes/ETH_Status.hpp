#ifndef ETH_STATUS_HPP
#define ETH_STATUS_HPP

#include "Network_Config.hpp"
#include "GPIO_Handler.hpp"

class ETH_Status {
private:
    NetworkConfig& ethInterface; 
    bool isInterfaceUp();        
    bool isRXActive();           

public:
    ETH_Status(NetworkConfig& config);

    void StartEthStatus(); 
};

#endif // ETH_STATUS_HPP
