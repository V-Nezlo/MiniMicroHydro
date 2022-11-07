//-----------------------------------------------------------------------
//  File        : SerialCommunicator.hpp
//  Created     : 6.10.2022
//  Modified    : 7.11.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Реализация настройки прибора через серийный порт

#ifndef SERIALCOMMUNICATOR
#define SERIALCOMMUNICATOR

#include "Arduino.h"
#include "Messages.hpp"
#include "PumpHandler.hpp"

class SerialCommunicator {

    enum class State {
        IDLE,
        SET_PUMPON_TIME,
        SET_PUMPOFF_TIME
    };

public:
    SerialCommunicator() :
        state{State::IDLE},
        pumpCallback{nullptr},
        eeCallback{nullptr},
        params{}
    {

    }

    void init(uint32_t aSpeed)
    {
        Serial.begin(aSpeed);
        Serial.setTimeout(10000);
        printf("Serial Communicator online!\r\n");
    }

    void setEeCallback(void (*aCallback)(Messages::HydroParams)) {
        eeCallback = aCallback;
    }

    void setPumpCallback(void (*aCallback)(Messages::HydroParams)) {
        pumpCallback = aCallback;
    }

void process()
{
    if(Serial.available()) {
        switch (state)
        {
        // Фаза 1 - ожидаем ввода команды config
        case State::IDLE:{
            String buffer = Serial.readStringUntil(0x0A);
            buffer.remove(buffer.length() - 1); // Remove CR

            if (buffer == "config") {
                printf("Entering config mode\r\n");
                state = State::SET_PUMPON_TIME;
                printf("Enter pump on time cycle in secs (max 4 number):\r\n");
            }
            break;
        }
        // Фаза 2 - ждем ввода параметра pumpOn
        case State::SET_PUMPON_TIME:{
            String buffer = Serial.readStringUntil(0x0a);
            params.pumpOnTime = buffer.toInt() * 1000;
            state = State::SET_PUMPOFF_TIME;
            printf("Pump on time was set on %lu seconds\r\n", params.pumpOnTime / 1000);
            printf("Enter pump off time cycle in secs (max 4 number):\r\n");
            break;
        }
        // Фаза 3 - ждем ввода параметра pumpOff
        case State::SET_PUMPOFF_TIME:{
            String buffer = Serial.readStringUntil(0x0a);
            params.pumpOffTime = buffer.toInt() * 1000;
            printf("Pump off time was set on %lu seconds\r\n", params.pumpOffTime / 1000);

            // Отправим по колбекам
            if (pumpCallback != nullptr) {
                pumpCallback(params);
            }
             if (eeCallback != nullptr) {
                eeCallback(params);
            }
            
            state = State::IDLE;
            printf("Config Done\r\n");
            break;
        }
        default:
            break;
        }
    }
}

private:
    State state;
    // Возможно стоит заменить на полноценную модель обсервера, но пока нет необходимости
    void (*pumpCallback)(Messages::HydroParams);
    void (*eeCallback)(Messages::HydroParams);
    
    Messages::HydroParams params; // Храним данные между фазами
};

#endif