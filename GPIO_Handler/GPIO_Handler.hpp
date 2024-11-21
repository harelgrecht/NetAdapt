#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H


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
