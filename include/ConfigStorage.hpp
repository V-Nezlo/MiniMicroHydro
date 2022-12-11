//-----------------------------------------------------------------------
//  File        : ConfigStorage.hpp
//  Created     : 06.12.2022
//  Modified    : 11.12.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Синглтон для хранения параметров

#ifndef CONFIGSTORAGE_HPP
#define CONFIGSTORAGE_HPP

#include "TimeContainer.hpp"
#include <stdio.h>

class ConfigStorage {
public:
    // Структура, хранимая в энергонезависимой памяти
    struct Config {
        uint32_t pumpOnTime{10000};
        uint32_t pumpOffTime{10000};
        TimeContainer lightOnTime;
        TimeContainer lightOffTime;
    } config;
    // Структура для данных, которые не нужно хранить в энергонезависимой памяти
    struct Temp {
        bool haveLight{false};
        TimeContainer settingTime;
    } temp;

    static ConfigStorage *instance()
    {
        static ConfigStorage instance;
        return &instance;
    }
private:
    ConfigStorage(){}
};

#endif