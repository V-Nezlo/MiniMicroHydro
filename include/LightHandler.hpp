//-----------------------------------------------------------------------
//  File        : PumpHandler.hpp
//  Created     : 3.12.2022
//  Modified    : 8.12.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Обработчик лампы или эквивалентного устройства

#ifndef LIGHTHANDLER_HPP
#define LIGHTHANDLER_HPP

#include "ConfigStorage.hpp"
#include "GpioWrapper.hpp"
#include <RTClib.h>
#include <Arduino.h>

template <typename Clock>
class LightHandler {
public:
    LightHandler(Gpio &aLightPin):
        lightPin{aLightPin},
        clock{},
        nextCheckTime{0}
    {
        // Скажем остальному коду что у нас есть обработчик для лампы
        ConfigStorage::instance()->temp.haveLight = true;
    }

    // Вызывать исклютельно из setup()
    void init()
    {
        if (!clock.begin()) {
            ConfigStorage::instance()->temp.haveLight = false;
            printf("Couldn't find RTC chip, light control disabled!\r\n");
        }
    }

    void process()
    {
        uint32_t currentTime = millis();

        if (currentTime > nextCheckTime) {
            nextCheckTime = currentTime + kUpdateTime;
        }
    }

    void update()
    {
        DateTime time(
            2022,
            12,
            1,
            ConfigStorage::instance()->temp.passingCurtime[0],
            ConfigStorage::instance()->temp.passingCurtime[1],
            ConfigStorage::instance()->temp.passingCurtime[2]
        );
        clock.adjust(time);
    }

private:
    Gpio &lightPin;
    Clock clock;
    uint32_t nextCheckTime;

    static constexpr uint32_t kUpdateTime{100};
};

#endif