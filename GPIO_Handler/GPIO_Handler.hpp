#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H

#include <iostream>
#include <fstream>
#include <string>

#define GPIO_ON 1
#define GPIO_OFF 0


class GPIO {

private:
	int PinNumber;
	std::string Direction;
	void GPIO_Export(int pin);
	void GPIO_Unexport(int pin);



public:
	GPIO(int pin, const std::string& direction);
	void GPIO_SetDirection(const std::string& direction);
	void GPIO_SetValue(int value);
	~GPIO();
};

#endif
