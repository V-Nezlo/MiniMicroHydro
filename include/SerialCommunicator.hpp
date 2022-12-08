//-----------------------------------------------------------------------
//  File        : SerialCommunicator.hpp
//  Created     : 6.10.2022
//  Modified    : 6.12.2022
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
    void setTimeObserver(void (*aTimeObserver)(const uint8_t*));
    void process();
private:
    State state;
    void (*observers[kMaxObservers])(void);
    void (*timeObserver)(const uint8_t*);
    uint8_t observerCounter;
};

#endif