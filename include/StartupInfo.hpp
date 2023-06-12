//-----------------------------------------------------------------------
//  File        : StartupInfo.hpp
//  Created     : 12.06.2023
//  Modified    : 12.06.2023
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Класс для вывода общей информации при старте системы

#ifndef STARTUPINFO
#define STARTUPINFO

#include "ConfigStorage.hpp"

class StartupInfo {
public:
    static void display()
    {
        printf("Current Light parameters: L.On %02d:%02d, L.Off %02d:%02d\r\n", 
            ConfigStorage::instance()->config.lightOnTime.hour(), 
            ConfigStorage::instance()->config.lightOnTime.minute(),
            ConfigStorage::instance()->config.lightOffTime.hour(),
            ConfigStorage::instance()->config.lightOnTime.minute());
        printf("Current Pump parameters: P.On = %lus, P.Off = %lus\r\n", 
            ConfigStorage::instance()->config.pumpOnTime / 1000,
            ConfigStorage::instance()->config.pumpOffTime / 1000
        );
    }
};

#endif
