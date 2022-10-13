//-----------------------------------------------------------------------
//  File        : PumpHandler.hpp
//  Created     : 6.10.2022
//  Modified    : 8.10.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Обработчик насоса

#ifndef PUMPHANDLER
#define PUMPHANDLER

#include <Arduino.h>
#include "GpioWrapper.hpp"
#include "Messages.hpp"

class PumpHandler {

    static constexpr uint32_t kButtonUpdateFreq{100};

public:
    PumpHandler(Gpio &aPump, Gpio &aButton, Gpio &aLedBlue) :
    pumpPin{aPump},
    button{aButton},
    ledBlue{aLedBlue},
    params{},
    state{State::PUMPOFF},
    nextSwitchTime{millis() + params.pumpOffTime},
    nextButtonCheckTime{millis()},
    permit{true}
    {

    }

    void updateParams(Messages::HydroParams &aParams)
    {
        params = aParams;
    }

    void updatePermit(bool aPermit)
    {
        permit = aPermit;
    }

    void process()
    {
        uint32_t currentTime = millis();

        if (currentTime > nextSwitchTime) {
            if (state == State::PUMPOFF) {
                state = State::PUMPON;
                nextSwitchTime = currentTime + params.pumpOnTime;
                if (permit) {
                    pumpPin.set(); // Включаем насос только если есть разрешение
                    ledBlue.set();
                }
            } else if (state == State::PUMPON) {
                state = State::PUMPOFF;
                nextSwitchTime = currentTime + params.pumpOffTime;
                pumpPin.reset(); // Отключаем насос без разрешение
                ledBlue.reset();
            }
        }
    }

private:

enum class State{
    PUMPON,
    PUMPOFF
};

Gpio &pumpPin;
Gpio &button;
Gpio &ledBlue;

Messages::HydroParams params;
State state;
uint32_t nextSwitchTime;
uint32_t nextButtonCheckTime;
bool permit;

};

#endif