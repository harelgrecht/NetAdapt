#ifndef KEEP_RUNNING_H
#define KEEP_RUNNING_H

#include <iostream>
#include <csignal>

bool KeepRunning = false;

void SignalHandler(int signal);

#endif