//-----------------------------------------------------------------------
//  File        : Main.cpp
//  Created     : 6.10.2022
//  Modified    : 8.12.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Main

#include "EeHandler.hpp"
#include "GpioWrapper.hpp"
#include "Initializator.hpp"
#include "SerialCommunicator.hpp"
#include "PumpHandler.hpp"
#include "FloatWaterLevel.hpp"
#include "LedIndicator.hpp"

#include "LightHandler.hpp"
#include "RTClib.h"

#include <Arduino.h>
#include <stdio.h>

// Инициализация серийного стрима через Serial 
static int serialfPutChar(const char ch, FILE*) 
{
	Serial.write(ch); 
	return 0;
}

static FILE *serialStream = fdevopen(serialfPutChar, nullptr); // fget не используется, поэтому nullptr

// Настройки пинов, менять под свою конфигурацию
Gpio ledGreen1(3, OUTPUT);
Gpio ledGreen2(4, OUTPUT);
Gpio ledOrange1(5, OUTPUT);
Gpio ledOrange2(6, OUTPUT);
Gpio ledRed(7, OUTPUT);
Gpio ledBlue(2, OUTPUT);

Gpio pump(12, OUTPUT);
Gpio button(A4, INPUT);
Gpio beeper(9, OUTPUT);
Gpio lightPin{13, OUTPUT};

Gpio waterLevel1(A2, INPUT);
Gpio waterLevel2(A1, INPUT);
Gpio waterLevel3(A0, INPUT);

SerialCommunicator communicator;
EeHandler eeprom;
LedIndicator indicator(&ledRed, &ledOrange1, &ledOrange2, &ledGreen1, &ledGreen2);
FloatLevelHandler floatLevel(500, waterLevel1, &waterLevel2, &waterLevel3, &indicator, &beeper);
PumpHandler pumpHandle(pump, nullptr, &ledBlue, &floatLevel);
LightHandler<RTC_DS1307> lightHandler(lightPin);

void postInit()
{
	Initializator initializator(&ledGreen1, &ledGreen2, &ledOrange1, &ledOrange2, &ledRed, &beeper, &ledBlue);
}

void setup()
{
	stdout = serialStream; // Указываем какой стрим указывать для printf
	communicator.init(115200); // Запускаем коммуникатор на скорости 115200
	lightHandler.init(); // Пробуем запустить RTC
	communicator.registerObserver([](){eeprom.update();});
	communicator.registerObserver([](){lightHandler.update();});
	EeHandler::readEeprom();
	postInit(); // Удалим инициализатор автоматически
}

void loop() {

	communicator.process();
	floatLevel.process();
	pumpHandle.process();

	delay(200);
}