//-----------------------------------------------------------------------
//  File        : GpioWrapper.hpp
//  Created     : 6.10.2022
//  Modified    : 8.10.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : ООП обертка для gpio

#ifndef GPIOWRAPPER
#define GPIOWRAPPER

#include <Arduino.h>

class Gpio {
public:
    Gpio(int aNum, int aType):
        number{aNum}
    {
		pinMode(number, aType);
    }
	// Чтобы не облажаться удалю ненужные конструкторы
	Gpio(const Gpio &) = delete;
	Gpio &operator=(const Gpio &) = delete;

	virtual ~Gpio()
	{
		// При удалении обьекта поставим пин в выход и низкое положение
		pinMode(number, OUTPUT);
		digitalWrite(number, LOW);
	}

	void set()
	{
		digitalWrite(number, HIGH);
	}

	void reset()
	{
		digitalWrite(number, LOW);
	}

	void setState(bool aState)
	{
		if (aState) {
			digitalWrite(number, HIGH);
		} else {
			digitalWrite(number, LOW);
		}
	}

	bool digitalRead()
	{
		return static_cast<bool>(::digitalRead(number));
	}

	int analogRead()
	{
		return ::analogRead(number);
	}

private:
    int number;
};

#endif