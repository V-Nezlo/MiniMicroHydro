//-----------------------------------------------------------------------
//  File        : GpioWrapper.cpp
//  Created     : 23.10.2022
//  Modified    : 23.10.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : ООП обертка для gpio

#include "GpioWrapper.hpp"

constexpr uint8_t Gpio::kPwmPins[];
    
Gpio::Gpio(int aNum, int aType):
    number{aNum}
{
	pinMode(number, aType);
}

Gpio::~Gpio()
{
	// При удалении обьекта поставим пин в выход и низкое положение
	pinMode(number, OUTPUT);
	digitalWrite(number, LOW);
}

void Gpio::set()
{
	digitalWrite(number, HIGH);
}

void Gpio::reset()
{
	digitalWrite(number, LOW);
}

void Gpio::setState(bool aState)
{
	if (aState) {
		digitalWrite(number, HIGH);
	} else {
		digitalWrite(number, LOW);
	}
}

bool Gpio::digitalRead()
{
	return static_cast<bool>(::digitalRead(number));
}

int Gpio::analogRead()
{
	return ::analogRead(number);
}

void Gpio::analogWrite(int aValue)
{
	if (checkPwmPinStatus()) {
		return ::analogWrite(number, aValue);
	} else {
		return ::digitalWrite(number, static_cast<uint8_t>(aValue));
	}

}

bool Gpio::checkPwmPinStatus()
{
	for (auto pos : kPwmPins) {
		if (number == pos) {
			return true;
		}
	}

	return false;
}
