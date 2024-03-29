//-----------------------------------------------------------------------
//  File        : PumpHandler.hpp
//  Created     : 7.11.2022
//  Modified    : 11.03.2023
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : 

#include "PumpHandler.hpp"

PumpHandler::PumpHandler(Gpio &aPump, Gpio *aButton, Gpio *aLedBlue, AbstractWaterLever *aLevel) :
    pumpPin{aPump},
    button{aButton},
    ledBlue{aLedBlue},
    state{State::PUMPOFF},
    nextSwitchTime{0}, 
    nextButtonCheckTime{TimeWrapper::milliseconds()},
    permit{true},
    initialized{false},
    level{aLevel}
{
    if (aLevel == nullptr) {
        printf("Pump water-is-low protection disabled");
    }
}

void PumpHandler::process()
{
    // Начинаем с состояния выкл и ждем полное время отключенного насоса
    if (!initialized) {
        initialized = true;
        nextSwitchTime = TimeWrapper::milliseconds() + ConfigStorage::instance()->config.pumpOffTime;
    }

    uint32_t currentTime = TimeWrapper::milliseconds();

    if (currentTime > nextSwitchTime) {
        if (state == State::PUMPOFF) {
            state = State::PUMPON;
            nextSwitchTime = currentTime + ConfigStorage::instance()->config.pumpOnTime;
            // Если есть защита от работы нахолостую - проверяем уровень воды
            if (level != nullptr) {
                if (level->getPermit()) {
                    pumpPin.set();
                    if (ledBlue != nullptr) {
                        ledBlue->set();
                    }
                }
            } else {
                // Если нет защиты от работы нахолостую - просто включаем
                pumpPin.set();
                if (ledBlue != nullptr) {
                    ledBlue->set();
                }
            }
        } else if (state == State::PUMPON) {
            state = State::PUMPOFF;
            nextSwitchTime = currentTime + ConfigStorage::instance()->config.pumpOffTime;
            pumpPin.reset(); // Отключаем насос
            if (ledBlue != nullptr) {
                ledBlue->reset();
            }
        }
    }

    if (button != nullptr && currentTime > nextButtonCheckTime) {
        nextButtonCheckTime = currentTime + kButtonUpdateFreq;
        if (button->digitalRead() && state == State::PUMPOFF) {
            do {
                pumpPin.set();
                if (ledBlue != nullptr) {
                    ledBlue->set();
                }
                delay(100);
            } while (button->digitalRead());
            pumpPin.reset();
            if (ledBlue != nullptr) {
                ledBlue->reset();
            }
        }
    }
}
