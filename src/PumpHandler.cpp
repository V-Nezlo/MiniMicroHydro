//-----------------------------------------------------------------------
//  File        : PumpHandler.hpp
//  Created     : 7.11.2022
//  Modified    : 11.10.2023
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : 

#include "PumpHandler.hpp"

PumpHandler::PumpHandler(Gpio &aPump, Gpio *aButton, Gpio *aLedBlue, AbstractWaterLever *aLevel) :
    pumpPin{aPump},
    button{aButton},
    ledBlue{aLedBlue},
    state{State::PUMPOFF},
    previousSwitchTime{0}, 
    previousButtonCheckTime{TimeWrapper::milliseconds()},
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
    uint32_t currentTime = TimeWrapper::milliseconds();

    // Защита от переполнения переменной со временем
    if (currentTime < previousSwitchTime) {
        previousSwitchTime = 0;
    }

    switch (state) {
        case State::PUMPON:
        // Если насос сейчас включен - смотрим, не пора ли выключать
            if (currentTime > previousSwitchTime + ConfigStorage::instance()->config.pumpOnTime) {
                previousSwitchTime = currentTime;
                // Переключаем состояние
                state = State::PUMPOFF;
                // Отключаем насос
                pumpPin.reset(); 
                // Переключаем лед если он сть
                if (ledBlue != nullptr) {
                    ledBlue->reset();
                }
            }
            break;
        case State::PUMPOFF:
            if (currentTime > previousSwitchTime + ConfigStorage::instance()->config.pumpOffTime) {
                previousSwitchTime = currentTime;
                // Переключаем состояние
                state = State::PUMPON;
                // Если есть защита от сухого включения - проверяем её, если нет - тупо включаем насос
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
            }
            break;
    }

    if (button != nullptr && currentTime > previousButtonCheckTime + kButtonUpdateFreq) {
        previousButtonCheckTime = currentTime;
        if (button->digitalRead() && state == State::PUMPOFF) {
            do {
                pumpPin.set();
                if (ledBlue != nullptr) {
                    ledBlue->set();
                }
                delay(100); // Не обернуто
            } while (button->digitalRead());
            pumpPin.reset();
            if (ledBlue != nullptr) {
                ledBlue->reset();
            }
        }
    }
}
