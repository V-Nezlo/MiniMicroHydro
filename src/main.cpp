//-----------------------------------------------------------------------
//  File        : Main.cpp
//  Created     : 6.10.2022
//  Modified    : 8.10.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Main

#include "EeHandler.hpp"
#include "Messages.hpp"
#include "GpioWrapper.hpp"
#include "Initializator.hpp"
#include "SerialCommunicator.hpp"
#include "PumpHandler.hpp"
#include "WaterLevelHandler.hpp"

#include <Arduino.h>
#include <stdio.h>

// Инициализация серийного стрима через Serial 
static int serialfPutChar(const char ch, FILE*) 
{
	Serial.write(ch); 
	return 0;
}

static FILE *serialStream = fdevopen(serialfPutChar, nullptr); // fget не используется, поэтому nullptr

Gpio ledGreen1(10, OUTPUT);
Gpio ledGreen2(7, OUTPUT);
Gpio ledOrange(9, OUTPUT);
Gpio ledRed(8, OUTPUT);
Gpio ledBlue(11, OUTPUT);

Gpio pump(2, OUTPUT);
Gpio button(6, INPUT);
Gpio beeper(5, OUTPUT);

Gpio waterLevel1(A2, INPUT);
Gpio waterLevel2(A0, INPUT);
Gpio waterLevel3(A1, INPUT);
Gpio waterPower(13, OUTPUT);

// Инициализаторы в глобале чтобы в лямбдах не указывать захват, нет поддержки функторов
PumpHandler pumpHandle(pump, button, ledBlue);
SerialCommunicator communicator;
WaterLeverHandler watLevHandle(100, ledGreen1, ledGreen2, ledOrange, ledRed, waterLevel1, 
	waterLevel2, waterLevel3, waterPower, beeper);
EeHandler eeprom;

void postInit()
{
	Initializator initializator(ledGreen1, ledGreen2, ledOrange, ledRed, beeper, ledBlue);
}

void setup()
{
	stdout = serialStream; // Указываем какой стрим указывать для printf
	communicator.init(115200); // Запускаем коммуникатор на скорости 115200

	// Колбеки на обновление параметров работы
	communicator.setPumpCallback([](Messages::HydroParams aParams){pumpHandle.updateParams(aParams);});
	communicator.setEeCallback([](Messages::HydroParams aParams){eeprom.writeEeprom(aParams);});

	// Колбек на разрешение включения насоса
	watLevHandle.setCallback([](bool aPermit){pumpHandle.updatePermit(aPermit);});

	// Загрузим значения из eeprom
	Messages::HydroParams params = eeprom.readEeprom();
	pumpHandle.updateParams(params); // Прокинем значения куда нужно

	postInit(); // Удалим инициализатор автоматически
}

void loop() {

	communicator.process();
	watLevHandle.process();
	pumpHandle.process();

	delay(10);
}