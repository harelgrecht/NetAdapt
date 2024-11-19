#include "Keep_Running.hpp"

void SignalHandler(int signal) {
    if (signal == SIGINT) {
        KeepRunning = false;
    }

}