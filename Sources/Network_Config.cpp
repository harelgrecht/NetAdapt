#include "../Includes/Network_Config.hpp"
#include <cstring>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdexcept>

NetworkConfig::NetworkConfig(const std::string& device, const std::string& ip, uint16_t port)
    : deviceName(device), ipAddress(ip), port(port), socketFD(-1) {
    configureInterface();
}

NetworkConfig::~NetworkConfig() {
    if (socketFD != -1) {
        close(socketFD);
        std::cout << "Socket closed for device: " << deviceName << std::endl;
    }
}

std::string NetworkConfig::getDeviceName() const {
    return deviceName;
}

int NetworkConfig::getSocketFD() const {
    return socketFD;
}

std::string NetworkConfig::getIPAddress() const {
    return ipAddress;
}

uint16_t NetworkConfig::getPort() const {
    return port;
}


void NetworkConfig::configureInterface() {
    socketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFD < 0) {
        throw std::runtime_error("Failed to create socket for device: " + deviceName);
    }

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, deviceName.c_str(), IFNAMSIZ);

    if (setsockopt(socketFD, SOL_SOCKET, SO_BINDTODEVICE, deviceName.c_str(), deviceName.size()) < 0) {
        close(socketFD);
        throw std::runtime_error("Failed to bind socket to device: " + deviceName);
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if (inet_pton(AF_INET, ipAddress.c_str(), &address.sin_addr) <= 0) {
        close(socketFD);
        throw std::runtime_error("Invalid IP address: " + ipAddress);
    }
}

void NetworkConfig::bindSocket() {
    if (bind(socketFD, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) < 0) {
        close(socketFD);
        throw std::runtime_error("Failed to bind socket to IP and port for device: " + deviceName);
    }

    std::cout << "Socket bound to " << ipAddress << ":" << port << " on device: " << deviceName << std::endl;
}
