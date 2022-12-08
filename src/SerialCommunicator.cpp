//-----------------------------------------------------------------------
//  File        : SerialCommunicator.hpp
//  Created     : 6.10.2022
//  Modified    : 8.12.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : 

#include "ConfigStorage.hpp"
#include "SerialCommunicator.hpp"
#include "PumpHandler.hpp"
#include <Arduino.h>

SerialCommunicator::SerialCommunicator() :
    state{State::Idle},
    observers{nullptr},
    timeObserver{nullptr},
    observerCounter{0}
{
}

void SerialCommunicator::init(uint32_t aSpeed)
{
    delay(500);
    Serial.begin(aSpeed);
    Serial.setTimeout(10000);
    printf("\r\nSerial Communicator online!\r\n");
    delay(500);
}

void SerialCommunicator::registerObserver(void (*aCallback)(void))
{
    if (observerCounter > kMaxObservers) {
        return;
    } else {
        observers[observerCounter++] = aCallback;
    }
}

void SerialCommunicator::setTimeObserver(void (*aTimeObserver)(const uint8_t*))
{
    timeObserver = aTimeObserver;
}

void SerialCommunicator::process()
{
if (Serial.available()) {
    String buffer = Serial.readStringUntil(0x0A);
    buffer.remove(buffer.length() - 1); // Remove CR

    // Команда исполняется в любом режиме и выходит из циклов настройки
    if (buffer == "exit") {
        printf("Force exit\r\n");
        state = State::Idle;
        return;
    }

    // Команда выводит справку
    if (buffer == "help" && state == State::Idle) {
        printf("Supported commands: pumpconfig, lightconfig, settime, time \r\n");
        return;
    }

    // Команда выводит текущее время если в системе есть часы реального времени
    if (buffer == "time" && ConfigStorage::instance()->temp.haveLight) {
        // TODO
    }

    // Переключатель для настройки
    switch (state) {
        case State::Idle:
            if (buffer == "pumpconfig") {
                state = State::SetPumpOnTime;
                printf("Entering pump config mode\r\n");
                printf("Enter pump on time cycle in secs:\r\n");
            } else if (buffer == "lightconfig") {
                if (ConfigStorage::instance()->temp.haveLight == true) {
                    printf("Enter light on time in format hh mm\r\n");
                    state = State::SetLightOnTime;
                } else {
                    printf("unsupported\r\n");
                }
            } else if (buffer == "settime") {
                if (ConfigStorage::instance()->temp.haveLight == true) {
                    state = State::SetTime;
                    printf("Enter new clock time in format hh mm ss\r\n");
                } else {
                    printf("unsupported\r\n");
                }
            } else {
                printf("unsupported\r\n");
            }
            break;
        case State::SetPumpOnTime:
            if (buffer.length() > 4) {
                printf("Wrong argument\r\n");
            } else {
                ConfigStorage::instance()->config.pumpOnTime = buffer.toInt() * 1000;
                state = State::SetPumpOffTime;
                printf("Enter pump off time cycle in secs:\r\n");
            }
            break;
        case State::SetPumpOffTime:
            if (buffer.length() > 4) {
                printf("Wrong argument\r\n");
            } else {
                ConfigStorage::instance()->config.pumpOffTime = buffer.toInt() * 1000;
                printf("New pump parameters - on cycle: %lu secs, off cycle: %lu\r\n", 
                    ConfigStorage::instance()->config.pumpOnTime / 1000, ConfigStorage::instance()->config.pumpOffTime / 1000);
                // Отправим по колбекам
                for (uint8_t i = 0; i < observerCounter; ++i) {
                    if (observers[i] != nullptr) {
                        observers[i]();
                    }
                }
                state = State::Idle;
                printf("Config done\r\n");
            }
            break;
        case State::SetTime:
            if (buffer.indexOf(0x20, 0) == 2 && buffer.indexOf(0x20, 3) == 5 && buffer.length() == 8)
            {
                uint8_t newTime[3] = {
                    static_cast<uint8_t>(buffer.substring(0, 2).toInt()),
                    static_cast<uint8_t>(buffer.substring(3, 5).toInt()),
                    static_cast<uint8_t>(buffer.substring(6, 8).toInt())};

                if (newTime[0] < 24 && newTime[1] < 60 && newTime[2] < 60) {
                    if (timeObserver != nullptr) {
                        timeObserver(newTime);
                    }
                    printf("New time = h:%u m:%u s:%u\r\n", newTime[0], newTime[1], newTime[2]);
                    state = State::Idle;
                } else {
                    printf("Wrong time\r\n");
                }
            } else {
                printf("Wrong argument\r\n");
            }
            break;
        case State::SetLightOnTime:
            if (buffer.length() == 5 && buffer.indexOf(0x20) == 2) {
                uint8_t newPumpOnTime[2] = {
                    static_cast<uint8_t>(buffer.substring(0, 2).toInt()), 
                    static_cast<uint8_t>(buffer.substring(3, 5).toInt())};

                if (newPumpOnTime[0] < 24 && newPumpOnTime[1] < 60) {
                    ConfigStorage::instance()->config.lightOnTime[0] = newPumpOnTime[0];
                    ConfigStorage::instance()->config.lightOnTime[1] = newPumpOnTime[1];
                    printf("Enter light off time in format hh mm\r\n");
                    state = State::SetLightOffTime;
                } else {
                    printf("Wrong time\r\n");
                }
            } else {
                printf("Wrong argument\r\n");
            }
            break;
        case State::SetLightOffTime:
            if (buffer.length() == 5 &&  buffer.indexOf(0x20) == 2) {
                uint8_t newPumpOffTime[2] = {
                    static_cast<uint8_t>(buffer.substring(0, 2).toInt()), 
                    static_cast<uint8_t>(buffer.substring(3, 5).toInt())};

                if (newPumpOffTime[0] < 24 && newPumpOffTime[1] < 60) {
                    ConfigStorage::instance()->config.lightOffTime[0] = newPumpOffTime[0];
                    ConfigStorage::instance()->config.lightOffTime[1] = newPumpOffTime[1];
                    printf("Light will be turned on in %u:%u and turned off in %u:%u \r\n",
                        ConfigStorage::instance()->config.lightOnTime[0],
                        ConfigStorage::instance()->config.lightOnTime[1],
                        ConfigStorage::instance()->config.lightOffTime[0],
                        ConfigStorage::instance()->config.lightOffTime[1]
                    );
                    // Отправим по колбекам
                    for (uint8_t i = 0; i < observerCounter; ++i) {
                        if (observers[i] != nullptr) {
                            observers[i]();
                        }
                    }
                    state = State::Idle;
                    printf("Config done\r\n");
                } else {
                    printf("Wrong time\r\n");
                }
            } else {
                printf("Wrong argument\r\n");
            }
            break;
        }
    }
}

