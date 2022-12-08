//-----------------------------------------------------------------------
//  File        : PumpHandler.hpp
//  Created     : 6.10.2022
//  Modified    : 7.11.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Обработчик насоса

#ifndef PUMPHANDLER
#define PUMPHANDLER

#include <Arduino.h>
#include "GpioWrapper.hpp"
#include "AbstractWaterLevel.hpp"
#include "ConfigStorage.hpp"

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
uint32_t nextSwitchTime;
uint32_t nextButtonCheckTime;
bool permit;

AbstractWaterLever *level;
};

#endif