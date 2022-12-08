//-----------------------------------------------------------------------
//  File        : PumpHandler.hpp
//  Created     : 7.11.2022
//  Modified    : 7.11.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : 

#include "PumpHandler.hpp"

PumpHandler::PumpHandler(Gpio &aPump, Gpio *aButton, Gpio *aLedBlue, AbstractWaterLever *aLevel) :
    pumpPin{aPump},
    button{aButton},
    ledBlue{aLedBlue},
    state{State::PUMPOFF},
    nextSwitchTime{millis() + ConfigStorage::instance()->config.pumpOffTime}, // Небольшой костыль, сюда прокидывается значение по умолчанию
    nextButtonCheckTime{millis()},
    permit{true},
    level{aLevel}
{
    if (aLevel == nullptr) {
        printf("Pump water-is-low protection disabled");
    }
}

void PumpHandler::process()
{
    uint32_t currentTime = millis();

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
