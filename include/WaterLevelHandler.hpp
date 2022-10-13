//-----------------------------------------------------------------------
//  File        : WaterLevelHandler.hpp
//  Created     : 6.10.2022
//  Modified    : 8.10.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Реализация работы электродного датчика уровня

#ifndef WATERLEVELHANDLER
#define WATERLEVELHANDLER

#include "GpioWrapper.hpp"
#include <Arduino.h>

class WaterLeverHandler {

public:
    WaterLeverHandler(uint32_t aUpdatePeriod, Gpio &aGreenLed1, Gpio &aGreenLed2, Gpio &aOrangeLed, Gpio &aRedLed,
        Gpio &aWaterLev1, Gpio &aWaterLev2, Gpio &aWaterLev3, Gpio &aWaterPower, Gpio &aBeeper):
    greenLed1{aGreenLed1},
    greenLed2{aGreenLed2},
    orangeLed{aOrangeLed},
    redLed{aRedLed},
    waterLev1{aWaterLev1},
    waterLev2{aWaterLev2},
    waterLev3{aWaterLev3},
	waterPower{aWaterPower},
	beeper{aBeeper},
	nextUpdateTime{millis()},
	updatePeriod{aUpdatePeriod},
	nextBeepTime{millis()},
	beepState{false},
	callback{nullptr}
    {
    }

	void setCallback(void (*aCallback)(bool)) 
	{
		if (aCallback != nullptr) {
			callback = aCallback;
		}
	}

	void process()
	{
		uint32_t currentTime = millis();
		bool error = false;

		if (currentTime > nextUpdateTime) {

			waterPower.set(); // Подаем питание на измерительные цепи

			greenLed1.setState(waterLev1.digitalRead());
			greenLed2.setState(waterLev2.digitalRead());
			orangeLed.setState(waterLev3.digitalRead());

			error = !waterLev3.digitalRead();
			redLed.setState(error);

			if (callback != nullptr) {
				callback(waterLev3.digitalRead());
			}

			waterPower.reset(); // Убираем питание с измерительных цепей чтобы не вызывать электролиз

			nextUpdateTime = currentTime + updatePeriod;
		}

		// Яростно кричим зумером если вода на исходе
		if (error) {
			if (currentTime > nextBeepTime) {
				if (beepState) {
					beepState = false;
					nextBeepTime = currentTime + beepOffTime;
				} else {
					beepState = true;
					nextBeepTime = currentTime + beepOnTime;
				}
				beeper.setState(beepState);
				}
			}
	}

private:

static constexpr uint32_t beepOnTime{100}; // Ms
static constexpr uint32_t beepOffTime{50000}; // Ms

Gpio &greenLed1;
Gpio &greenLed2;
Gpio &orangeLed;
Gpio &redLed;

Gpio &waterLev1;
Gpio &waterLev2;
Gpio &waterLev3;
Gpio &waterPower;

Gpio &beeper;

uint32_t nextUpdateTime; // Ms
uint32_t updatePeriod; // Ms
uint32_t nextBeepTime; // Ms
bool beepState;

void (*callback)(bool);

};

#endif