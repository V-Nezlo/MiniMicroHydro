//-----------------------------------------------------------------------
//  File        : SerialCommunicator.hpp
//  Created     : 6.10.2022
//  Modified    : 5.12.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Реализация настройки прибора через серийный порт

#ifndef SERIALCOMMUNICATOR
#define SERIALCOMMUNICATOR

#include "Arduino.h"
#include "Messages.hpp"
#include "PumpHandler.hpp"

class SerialCommunicator {

    enum class State {
        Idle,
        SetPumpOnTime,
        SetPumpOffTime
    };

public:
    SerialCommunicator() :
        state{State::Idle},
        observers{nullptr},
        observerCounter{0},
        params{}
    {

    }

    void init(uint32_t aSpeed)
    {
        Serial.begin(aSpeed);
        Serial.setTimeout(10000);
        printf("Serial Communicator online!\r\n");
    }

    void registerObserver(void (*aCallback)(Messages::HydroParams))
    {
        observers[observerCounter++] = aCallback;
    }

void process()
{
    if(Serial.available()) {
        String buffer = Serial.readStringUntil(0x0A);
        buffer.remove(buffer.length() - 1); // Remove CR

        switch (state) {
            case State::Idle:
                if (buffer == "pumpconfig") {
                    state = State::SetPumpOnTime;
                    printf("Entering pump config mode\r\n");
                    printf("Enter pump on time cycle in secs (max 4 number):\r\n");
                } else if (buffer == "lightconfig") {
                    printf("unsupported\r\n");
                } else if (buffer == "settime") {
                    printf("unsupported\r\n");
                }
                break;
            case State::SetPumpOnTime:
                params.pumpOnTime = buffer.toInt() * 1000;
                state = State::SetPumpOffTime;
                printf("Enter pump off time cycle in secs (max 4 number):\r\n");
                break;
            case State::SetPumpOffTime:
                params.pumpOffTime = buffer.toInt() * 1000;
                printf("New pump parameters - on cycle: %lu secs, off cycle: %lu \r\n", params.pumpOnTime / 1000, params.pumpOffTime / 1000);
                // Отправим по колбекам
                for (uint8_t i = 0; i < observerCounter; ++i) {
                    if (observers[i] != nullptr) {
                        observers[i](params);
                    }
                }
                state = State::Idle;
                printf("Config done");
                break;
        }
    }
}

private:
    State state;
    void (*observers[2])(Messages::HydroParams);
    uint8_t observerCounter;

    Messages::HydroParams params; // Храним данные между фазами
};

#endif