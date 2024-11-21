#include "../MUST.HPP"


GPIO::GPIO(int pin, const std::string& direction) : PinNumber(pin), Direction(direction) {
	GPIO_Export(pin);
	GPIO_SetDirection(direction);
}

void GPIO::GPIO_Export(int pin) {
	std::ofstream export_file("/sys/class/gpio/export");
	if (export_file) {
		export_file << pin;
		export_file.close();
	}
}

void GPIO::GPIO_Unexport(int pin) {
	std::ofstream unexport_file("/sys/class/gpio/unexport");
	if (unexport_file) {
		unexport_file << pin;
		unexport_file.close();
	}
}

void GPIO::GPIO_SetDirection(const std::string& direction) {
	std::ofstream direction_file("/sys/class/gpio/gpio" + std::to_string(PinNumber) + "/direction");
	if (direction_file) {
		direction_file << direction;
		direction_file.close();
	}
}


void GPIO::GPIO_SetValue(int value) {
	std::ofstream value_file("/sys/class/gpio/gpio" + std::to_string(PinNumber) + "/value");
	if (value_file) {
		value_file << value;
		value_file.close();
	}
}

GPIO::~GPIO() {
	GPIO_Unexport(PinNumber);
}


