//-----------------------------------------------------------------------
//  File        : Main.cpp
//  Created     : 6.10.2022
//  Modified    : 5.12.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Main

#include "EeHandler.hpp"
#include "Messages.hpp"
#include "GpioWrapper.hpp"
#include "Initializator.hpp"
#include "SerialCommunicator.hpp"
#include "PumpHandler.hpp"
#include "FloatWaterLevel.hpp"
#include "LedIndicator.hpp"

#include <Arduino.h>
#include <stdio.h>

// Инициализация серийного стрима через Serial 
static int serialfPutChar(const char ch, FILE*) 
{
	Serial.write(ch); 
	return 0;
}

static FILE *serialStream = fdevopen(serialfPutChar, nullptr); // fget не используется, поэтому nullptr

Gpio ledGreen1(3, OUTPUT);
Gpio ledGreen2(4, OUTPUT);
Gpio ledOrange1(5, OUTPUT);
Gpio ledOrange2(6, OUTPUT);
Gpio ledRed(7, OUTPUT);
Gpio ledBlue(2, OUTPUT);

Gpio pump(12, OUTPUT);
Gpio button(A4, INPUT);
Gpio beeper(9, OUTPUT);

Gpio waterLevel1(A2, INPUT);
Gpio waterLevel2(A1, INPUT);
Gpio waterLevel3(A0, INPUT);

SerialCommunicator communicator;
EeHandler eeprom;
LedIndicator indicator(&ledRed, &ledOrange1, &ledOrange2, &ledGreen1, &ledGreen2);
FloatLevelHandler floatLevel(100, waterLevel1, &waterLevel2, &waterLevel3, &indicator, &beeper);
PumpHandler pumpHandle(pump, &button, &ledBlue, &floatLevel);

void postInit()
{
	Initializator initializator(&ledGreen1, &ledGreen2, &ledOrange1, &ledOrange2, &ledRed, &beeper, &ledBlue);
}

void setup()
{
	stdout = serialStream; // Указываем какой стрим указывать для printf
	communicator.init(115200); // Запускаем коммуникатор на скорости 115200

	// Колбеки на обновление параметров работы
	communicator.registerObserver([](Messages::HydroParams aParams){eeprom.writeEeprom(aParams);});
	communicator.registerObserver([](Messages::HydroParams aParams){pumpHandle.updateParams(aParams);});

	// Загрузим значения из eeprom
	Messages::HydroParams params = eeprom.readEeprom();
	pumpHandle.updateParams(params); // Прокинем значения куда нужно

	postInit(); // Удалим инициализатор автоматически
}

void loop() {

	communicator.process();
	floatLevel.process();
	pumpHandle.process();

	delay(10);
}