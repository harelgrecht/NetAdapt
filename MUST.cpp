#include "MUST.HPP"

int main() {
    PacketCapture Capture(RECIVER_DEVICE,RECIVER_IP);
    std::thread CapturingThread([&Capture](){
        if(!Capture.StartCapture(filter)) {
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