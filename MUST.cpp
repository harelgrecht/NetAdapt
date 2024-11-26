#include "MUST.HPP"

int main() {

    GPIO UF1(GPIO1, GPIO_OUT);
    GPIO UF2(GPIO2, GPIO_OUT);

    UF1.SetValue(1);
    UF2.SetValue(1);

    PacketCapture Capture(RECIVER_DEVICE, RECIVER_IP);
    std::thread CapturingThread([&Capture](){
        if(!Capture.StartCapture(filter)) {
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

    return 0;
}