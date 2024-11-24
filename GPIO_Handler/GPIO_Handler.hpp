#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H


class GPIO {

private:
	int PinNumber;
	std::string Direction;
	void ExportGPIO(int pin);
	void UnexportGPIO(int pin);

public:
	GPIO(int pin, const std::string& direction);
	~GPIO();
	void SetDirection(const std::string& direction);
	void SetValue(int value);
	void WriteToFile(const std::string& FilePath, const std::string& Data);
};

#endif
