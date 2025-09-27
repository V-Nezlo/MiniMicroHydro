//-----------------------------------------------------------------------
//  File        : PumpHandler.hpp
//  Created     : 6.10.2022
//  Modified    : 11.10.2023
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Обработчик насоса

#ifndef PUMPHANDLER
#define PUMPHANDLER

#include "AbstractWaterLevel.hpp"
#include "ConfigStorage.hpp"
#include "GpioWrapper.hpp"
#include "TimeWrapper.hpp"
#include <Arduino.h>

class PumpHandler {
    static constexpr uint32_t kButtonUpdateFreq{100};
public:
    PumpHandler(Gpio &aPump, Gpio *aButton, Gpio *aLedBlue, AbstractWaterLever *aLevel = nullptr);
    void process();
private:

enum class State{
    PUMPON,
    PUMPOFF
};

Gpio &pumpPin;
Gpio *button;
Gpio *ledBlue;

State state;
uint32_t previousSwitchTime;
uint32_t previousButtonCheckTime;
uint32_t previousTime;
bool permit;
bool initialized;

AbstractWaterLever *level;
};

#endif