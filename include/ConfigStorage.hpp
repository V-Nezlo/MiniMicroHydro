//-----------------------------------------------------------------------
//  File        : ConfigStorage.hpp
//  Created     : 06.12.2022
//  Modified    : 08.12.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Синглтон для хранения параметров

#ifndef CONFIGSTORAGE_HPP
#define CONFIGSTORAGE_HPP

#include <stdio.h>

class ConfigStorage {
public:
    // Структура, хранимая в энергонезависимой памяти
    struct Config {
        uint32_t pumpOnTime{10000};
        uint32_t pumpOffTime{10000};
        uint8_t lightOnTime[2]{10, 0};
        uint8_t lightOffTime[2]{20, 0};
    } config;
    // Структура для данных, которые не нужно хранить в энергонезависимой памяти
    struct Temp {
        bool haveLight{false};
        uint8_t passingCurtime[3]; 
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