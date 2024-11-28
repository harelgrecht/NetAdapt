#include "../Includes/Libraries.hpp"

bool KeepRunning = true;

void SignalHandler(int signal) {
    if (signal == SIGINT) 
        KeepRunning = false;
}

int main() {
    
    std::signal(SIGINT, SignalHandler);

    PacketCapture Capture(SOURCE_DEVICE, SOURCE_IP);
    std::thread CapturingThread([&Capture] () {
        if(!Capture.StartCapture(ETH_FILTER)) {
            std::cerr << "Failed to start capturing packets" << std::endl;
            return -1;
        }
    });

    PacketProcess PacketProcessor;
    std::thread ProccesingThread([&PacketProcessor]() {
        while(KeepRunning) {
            PacketProcessor.ProcessStart();
        }
    });

    PacketSend Send(DESTINATION_DEVICE);
    std::thread SendingThread([&Send]() {
        Send.FetchPacketFromQueue();
        Send.CreatePacket(SOURCE_IP, DESTINATION_IP, SOURCE_PORT, DESTINATION_PORT);
        Send.SendPacket();
    });


    CapturingThread.join();
    ProccesingThread.join();
    SendingThread.join();
    return 0;
}