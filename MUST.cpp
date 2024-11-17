#include "GPIO_Handler/GPIO_Handler.hpp"
#include "Packet_Capture/Packet_Capture.hpp"

int main() {


    /*GPIO make the OS crash*/
    // std::cout << "Setting up gpio to '1'" << std::endl;
    // GPIO Green(506,GPIO_OUT);
    // Green.GPIO_SetValue(GPIO_ON);
    // GPIO Orange(507,GPIO_OUT);
    // Orange.GPIO_SetValue(GPIO_ON);


    std::string device = "eth0";
    std::string ip = "192.168.1.1";
    PacketCapture packetCapture(device,ip);
    if(!packetCapture.StartCapture()){
        std::cerr << "Failed to start capturing packets" << std::endl;
    }



    return 0;
}