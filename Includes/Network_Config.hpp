#ifndef NETWORK_CONFIG_HPP
#define NETWORK_CONFIG_HPP

#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>

class NetworkConfig {
public:
    NetworkConfig(const std::string& device, const std::string& ip, uint16_t port);
    ~NetworkConfig();

    std::string getDeviceName() const;
    void bindSocket();

private:
    std::string deviceName;
    std::string ipAddress;
    uint16_t port;
    int socketFD;
    struct sockaddr_in address;

    void configureInterface();
};

#endif // NETWORK_CONFIG_HPP
