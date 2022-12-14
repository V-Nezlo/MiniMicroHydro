//-----------------------------------------------------------------------
//  File        : PumpHandler.hpp
//  Created     : 3.12.2022
//  Modified    : 11.12.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Обработчик лампы или эквивалентного устройства

#ifndef LIGHTHANDLER_HPP
#define LIGHTHANDLER_HPP

#include "ConfigStorage.hpp"
#include "GpioWrapper.hpp"
#include "TimeWrapper.hpp"
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

    void init()
    {
        if (!clock.begin()) {
            ConfigStorage::instance()->temp.haveLight = false;
            printf("Couldn't find RTC chip, light control disabled!\r\n");
        }
    }

    void process()
    {
        uint32_t currentTime = TimeWrapper::milliseconds();

        if (currentTime > nextCheckTime) {
            nextCheckTime = currentTime + kUpdateTime;

            DateTime time = clock.now();
            TimeContainer curContainedTime(time.hour(), time.minute(), time.second());

            if (curContainedTime < ConfigStorage::instance()->config.lightOnTime || 
                curContainedTime > ConfigStorage::instance()->config.lightOffTime) {
                lightPin.reset();
            } else {
                lightPin.set();
            }
        }
    }

    void update()
    {
        // Проверим, есть ли RTC и есть ли в структуре с временем время
        if (ConfigStorage::instance()->temp.haveLight && !ConfigStorage::instance()->temp.settingTime.isEmpty()) {
            DateTime time(
                2022,
                12,
                1,
                ConfigStorage::instance()->temp.settingTime.hour(),
                ConfigStorage::instance()->temp.settingTime.minute(),
                ConfigStorage::instance()->temp.settingTime.second()
            );
            clock.adjust(time);

            // Явно скажем структуре что она пуста
            ConfigStorage::instance()->temp.settingTime.clear();
        }
    }

private:
    Gpio &lightPin;
    Clock clock;
    uint32_t nextCheckTime;

    static constexpr uint32_t kUpdateTime{100};
};

#endif