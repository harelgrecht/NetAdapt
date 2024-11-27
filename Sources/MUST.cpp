#include "../Includes/Libraries.hpp"

bool KeepRunning = true;

void SignalHandler(int signal) {
    if (signal == SIGINT) 
        KeepRunning = false;
}

int main() {
    
    std::signal(SIGINT, SignalHandler);

    PacketCapture Capture(RECIVER_DEVICE, RECIVER_IP);
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

    PacketSend Send;
    std::thread SendingThread([&Send]() {
        Send.SendPacket(DESTINATION_IP, DESTINATION_PORT, DESTINATION_DEVICE);
    });


    CapturingThread.join();
    ProccesingThread.join();
    SendingThread.join();
    return 0;
}