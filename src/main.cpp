//-----------------------------------------------------------------------
//  File        : Main.cpp
//  Created     : 6.10.2022
//  Modified    : 12.06.2023
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
#include "StartupInfo.hpp"

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
Gpio ledBlue(8, OUTPUT);
Gpio ledGreen1(9, OUTPUT);
Gpio ledGreen2(10, OUTPUT);
Gpio ledOrange1(11, OUTPUT);
Gpio ledOrange2(12, OUTPUT);
Gpio ledRed(13, OUTPUT);

Gpio pump(3, OUTPUT);
Gpio button(A3, INPUT);
Gpio beeper(A1, OUTPUT);
Gpio lightPin{2, OUTPUT};

Gpio waterLevel1(6, INPUT_PULLUP);
Gpio waterLevel2(5, INPUT_PULLUP);
Gpio waterLevel3(4, INPUT_PULLUP);

SerialCommunicator communicator;
EeHandler eeprom;
LedIndicator indicator(&ledRed, &ledOrange1, &ledOrange2, &ledGreen1, &ledGreen2);
FloatLevelHandler floatLevel(500, true, waterLevel1, &waterLevel2, &waterLevel3, &indicator, &beeper);
PumpHandler pumpHandle(pump, nullptr, &ledBlue, &floatLevel);
LightHandler<RTC_DS1307> lightHandler(500, lightPin);

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
	StartupInfo::display(); // Отобразим текущие параметры
}

void loop() {

	communicator.process();
	floatLevel.process();
	pumpHandle.process();
	lightHandler.process();

	delay(200);
}