//-----------------------------------------------------------------------
//  File        : SerialCommunicator.hpp
//  Created     : 6.10.2022
//  Modified    : 11.12.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Реализация настройки прибора через серийный порт

#ifndef SERIALCOMMUNICATOR
#define SERIALCOMMUNICATOR

#include "Arduino.h"
#include "PumpHandler.hpp"
#include "ConfigStorage.hpp"

class SerialCommunicator {
    enum class State {
        Idle,
        SetPumpOnTime,
        SetPumpOffTime,
        SetTime,
        SetLightOnTime,
        SetLightOffTime
    };

    static constexpr uint8_t kMaxObservers{2};
public:
    SerialCommunicator();
    void init(uint32_t aSpeed);
    void registerObserver(void (*aCallback)(void));
    void process();
private:
    void callbacks();
    State state;
    void (*observers[kMaxObservers])(void);
    uint8_t observerCounter;
};

#endif