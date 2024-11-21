#include "../MUST.hpp"

void SignalHandler(int signal) {
    if (signal == SIGINT) {
        KeepRunning = false;
    }

}