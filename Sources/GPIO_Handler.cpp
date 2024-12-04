#include "../Includes/GPIO_Handler.hpp"
#include "../Includes/Global_Defines.hpp"


GPIO::GPIO(int pin, const std::string& direction) : PinNumber(pin), Direction(direction) {
	ExportGPIO(pin);
	SetDirection(direction);
}

void GPIO::ExportGPIO(int pin) {
	WriteToFile(GPIO_EXPORT_PATH, std::to_string(pin));
}

void GPIO::UnexportGPIO(int pin) {
	WriteToFile(GPIO_UNEXPORT_PATH, std::to_string(pin));
}

void GPIO::SetDirection(const std::string& direction) {
	WriteToFile(GPIO_DIRECTION_PATH, direction);
}


void GPIO::SetValue(int value) {
	WriteToFile(GPIO_VALUE_PATH, std::to_string(value));
}

GPIO::~GPIO() {
	UnexportGPIO(PinNumber);
}

void GPIO::BlinkLed() {
	SetValue(GPIO_ON);
	std::this_thread::sleep_for(std::chrono::milliseconds(LED_BLINK_DURATION_MS));
	SetValue(GPIO_OFF);
}



void GPIO::WriteToFile(const std::string& FilePath, const std::string& Data) {
	std::ofstream file(FilePath);
	if(!file.is_open() || file.fail()) {
		std::cerr << "Failed to open the file" + FilePath << std::endl;
		return;
	}
	file << Data;
}


