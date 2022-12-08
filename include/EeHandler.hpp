//-----------------------------------------------------------------------
//  File        : EeHandler.hpp
//  Created     : 7.10.2022
//  Modified    : 7.11.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Мне лень адаптировать свою библиотеку, работающую под ESP32, под этот недокамень, поэтому будет так

#ifndef EEHANDLER
#define EEHANDLER

#include "ConfigStorage.hpp"
#include <Arduino.h>
#include <EEPROM.h>

class EeHandler {
public:
    static void update()
    {
        EEPROM.begin();
        EEPROM.put(0, ConfigStorage::instance()->config);
        EEPROM.end();
    }

    static void readEeprom()
    {
        ConfigStorage::Config newConfig;
        EEPROM.begin();
        EEPROM.get(0, newConfig);
        EEPROM.end();
        ConfigStorage::instance()->config = newConfig;
    }
};

#endif