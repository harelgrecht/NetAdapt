#include "../Includes/GPIO_Handler.hpp"
#include "../Includes/Global_Defines.hpp"

GPIO::GPIO(int pin, const std::string& direction) : PinNumber(pin), Direction(direction) {
#ifndef MOCK_UP
    ExportGPIO(pin);
    SetDirection(direction);
#endif
}

void GPIO::ExportGPIO(int pin) {
#ifndef MOCK_UP
    WriteToFile(GPIO_EXPORT_PATH, std::to_string(pin));
#endif
}

void GPIO::UnexportGPIO(int pin) {
#ifndef MOCK_UP
    WriteToFile(GPIO_UNEXPORT_PATH, std::to_string(pin));
#endif
}

void GPIO::SetDirection(const std::string& direction) {
#ifndef MOCK_UP  
    WriteToFile(GPIO_DIRECTION_PATH, direction);
#endif
}

void GPIO::SetValue(int value) {
#ifndef MOCK_UP
    WriteToFile(GPIO_VALUE_PATH, std::to_string(value));
#endif
}

void GPIO::BlinkLed() {
#ifndef MOCK_UP
    SetValue(GPIO_ON);
    SetValue(GPIO_OFF);
#endif
}

GPIO::~GPIO() {
#ifndef MOCK_UP
    UnexportGPIO(PinNumber);
#endif
}

void GPIO::WriteToFile(const std::string& FilePath, const std::string& Data) {
#ifndef MOCK_UP
    std::ofstream file(FilePath);
    if (!file.is_open() || file.fail()) {
        std::cerr << "Failed to open the file " + FilePath << std::endl;
        return;
    }
    file << Data;
#endif
}
