#ifndef LED_HANDLER_HPP
#define LED_HANDLER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>  


#define GPIO_ON 1
#define GPIO_OFF 0
#define GPIO_OUT "OUT"
#define GPIO_IN "in"
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define GPIO_UNEXPORT_PATH "/sys/class/gpio/unexport"
#define GPIO_DIRECTION_PATH "/sys/class/gpio/gpio%d/direction"
#define GPIO_VALUE_PATH "/sys/class/gpio/gpio%d/value"

class ledHandler {
    private:
        bool writeToFile(const std::string& filePath, const std::string& data);
        std::string formatPath(const char* pathTemplate) const;
        
        public:
        int pinNumber;
        ledHandler(int ledPin, const std::string& direction);
        ~ledHandler();
        
        int turnOn();
        int turnOff();
        int blinkLed();
};


#endif