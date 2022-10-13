//-----------------------------------------------------------------------
//  File        : EeHandler.hpp
//  Created     : 7.10.2022
//  Modified    : 8.10.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Мне лень адаптировать свою библиотеку, работающую под ESP32, под этот недокамень, поэтому будет так

#ifndef EEHANDLER
#define EEHANDLER

#include <Arduino.h>
#include <EEPROM.h>
#include "Messages.hpp"

class EeHandler {
public:
    void writeEeprom(Messages::HydroParams aParams)
    {
        EEPROM.begin();
        printf("EEprom writing values: %lu,%lu", aParams.pumpOnTime, aParams.pumpOffTime);
        EEPROM.put(0, aParams);
        EEPROM.end();
    }

    Messages::HydroParams readEeprom()
    {
        Messages::HydroParams params;
        EEPROM.begin();
        EEPROM.get(0, params);
        EEPROM.end();
        return params;
        
    }
};

#endif