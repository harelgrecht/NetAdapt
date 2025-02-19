#include "ledHandler.hpp"
#include <thread>
#include <chrono>

ledHandler::ledHandler(int ledPin, const std::string& direction) : pinNumber(ledPin) {
    writeToFile(GPIO_EXPORT_PATH, std::to_string(pinNumber));  
    writeToFile(formatPath(GPIO_DIRECTION_PATH), direction);
}

ledHandler::~ledHandler() {
    writeToFile(GPIO_UNEXPORT_PATH, std::to_string(pinNumber));
}

int ledHandler::turnOn() {
    return writeToFile(formatPath(GPIO_VALUE_PATH), std::to_string(GPIO_ON));
}

int ledHandler::turnOff() {
    return writeToFile(formatPath(GPIO_VALUE_PATH), std::to_string(GPIO_OFF));
}

int ledHandler::blinkLed() {
    std::string valuePath = formatPath(GPIO_VALUE_PATH);
    if (!writeToFile(valuePath, std::to_string(GPIO_ON))) {
        return false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return writeToFile(valuePath, std::to_string(GPIO_OFF));
}

std::string ledHandler::formatPath(const char* pathTemplate) const {
    char formattedPath[100];
    snprintf(formattedPath, sizeof(formattedPath), pathTemplate, pinNumber);
    return std::string(formattedPath);
}

bool ledHandler::writeToFile(const std::string& filePath, const std::string& data) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open: " << filePath << std::endl;
        return false;
    }
    file << data;
    if (file.fail()) {
        std::cerr << "Failed to write to: " << filePath << std::endl;
        return false;
    }
    return true;
}
