#include "MUST.HPP"

int main() {
    PacketCapture Capture(ReciverDevice,ReciverIP);
    std::thread CapturingThread([&Capture](){
        if(!Capture.StartCapture()) {
            std::cerr << "Failed to start capturing packets" << std::endl;
            return -1;
        }
    });

    PacketProcess Process;
    std::thread ProccesingThread([&Process]() {
        while(KeepRunning) {
            Process.GetPackets();
            Process.CompressPacket();
            Process.StorePackets();
        }
    });

    PacketSend Send;
    std::thread SendingThread([&Send]() {

    });





    return 0;
}