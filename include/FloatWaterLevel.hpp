//-----------------------------------------------------------------------
//  File        : WaterLevelHandler.hpp
//  Created     : 6.10.2022
//  Modified    : 10.12.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Реализация работы электродного датчика уровня

#ifndef FLOATWATERLEVEL
#define FLOATWATERLEVEL

#include "AbstractWaterLevel.hpp"
#include "AbstractWaterIndicator.hpp"
#include "Constants.hpp"
#include "GpioWrapper.hpp"
#include "TimeWrapper.hpp"
#include <Arduino.h>
#include "assert.h"

class FloatLevelHandler : public AbstractWaterLever {

public:
    FloatLevelHandler(uint32_t aUpdatePeriod, Gpio &aWaterLev1, Gpio *aWaterLev2 = nullptr, Gpio *aWaterLev3 = nullptr, 
		AbstractWaterIndicator *aIndicator = nullptr, Gpio *aBeeper = nullptr);
	FloatLevelHandler(const FloatLevelHandler &) = delete;
	FloatLevelHandler operator=(const FloatLevelHandler &) = delete;
	virtual ~FloatLevelHandler();

	void process() override;
	uint8_t getLevel() override;
	bool getPermit() override;

private:
static constexpr uint32_t kBeepOnTime{200}; // Ms
static constexpr uint32_t kBeepOffTime{30000}; // Ms

Gpio &waterLev1; // Защита от пропихивания нульпоинтера на место важного элемента
Gpio *waterLev2;
Gpio *waterLev3;
Gpio *beeper;

enum class Type{
	ThreeSensors,
	TwoSensors,
	OneSensors
} type;

uint32_t nextUpdateTime; // Ms
uint32_t updatePeriod; // Ms
uint32_t nextBeepTime; // Ms
bool isFloatLevel;

uint8_t currentProcents;
bool permit;
bool beepState;

AbstractWaterIndicator *indicator;
};

#endif