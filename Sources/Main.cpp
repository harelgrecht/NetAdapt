#include "../Includes/Global_Defines.hpp"
#include "../Includes/ETH_Status.hpp"
#include "../Includes/Packet_Capture.hpp"
#include "../Includes/Packet_Process.hpp"
#include "../Includes/Packet_Send.hpp"
#include "../Includes/Exceptions.hpp"
#include "../Includes/Network_Config.hpp"

#include <csignal>
#include <thread>
#include <iostream>
#include <stdexcept>
#include <atomic>

std::atomic<bool> KeepRunning(true);

void SignalHandler(int signal) {
    if (signal == SIGINT) 
        KeepRunning = false;
}

int main() {
    
    std::signal(SIGINT, SignalHandler);

    NetworkConfig eth0(SOURCE_DEVICE, SOURCE_IP, SOURCE_PORT);
    eth0.bindSocket();

    NetworkConfig eth1(DESTINATION_DEVICE, DESTINATION_IP, DESTINATION_PORT);
    eth1.bindSocket();

    ETH_Status eth0Status(eth0);
    ETH_Status eth1Status(eth1);
    std::thread StatusThread([&]() {
        while(KeepRunning)
            eth0Status.StartEthStatus();
            eth1Status.StartEthStatus();
    });

#ifdef MOCK_UP
    std::thread CapturingThread([] () {
        SimulatePacketInjector();
    });
#else
        PacketCapture PacketSniffer(eth0);
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

    PacketSend UdpSender(eth1, DESTINATION_IP, DESTINATION_PORT);
    std::thread SendingThread([&UdpSender]() {
        UdpSender.SendPacket();
    });

    StatusThread.join();
    CapturingThread.join();
    ProccesingThread.join();
    SendingThread.join();
    return 0;
}