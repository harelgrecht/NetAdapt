#include "../Includes/Global_Defines.hpp"
#include "../Includes/ETH_Status.hpp"
#include "../Includes/Packet_Capture.hpp"
#include "../Includes/Packet_Process.hpp"
#include "../Includes/Packet_Send.hpp"
#include "../Includes/Exceptions.hpp"

#include <csignal>
#include <thread>
#include <iostream>
#include <stdexcept>
bool KeepRunning = true;

void SignalHandler(int signal) {
    if (signal == SIGINT) 
        KeepRunning = false;
}

int main() {
    
    std::signal(SIGINT, SignalHandler);

    ETHStatus Status;
    std::thread StatusThread([&Status]() {
        while(KeepRunning)
            Status.StartEthStatus();
    });



#ifdef MOCK_UP
    std::thread CapturingThread([] () {
        SimulatePacketInjector();
    });
#else
        PacketCapture PacketSniffer(SOURCE_DEVICE, SOURCE_IP);
        std::thread CapturingThread([&PacketSniffer] () {
        try{
            PacketSniffer.StartCapture(ETH_FILTER);
        }
        catch(NetworkException& e){
            std::cerr << "Failed in capturing packets" << e.what() << std::endl;
        }
    });
#endif

    PacketProcess PacketProcessor;
    std::thread ProccesingThread([&PacketProcessor]() {
        while(KeepRunning) {
            PacketProcessor.ProcessStart();
        }
    });

    PacketSend UdpSender(DESTINATION_DEVICE);
    std::thread SendingThread([&UdpSender]() {
        UdpSender.SendPacket(SOURCE_IP, DESTINATION_IP, SOURCE_PORT, DESTINATION_PORT);
    });

    StatusThread.join();
    CapturingThread.join();
    ProccesingThread.join();
    SendingThread.join();
    return 0;
}