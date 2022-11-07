//-----------------------------------------------------------------------
//  File        : AbstractWaterLevel.hpp
//  Created     : 23.10.2022
//  Modified    : 23.10.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Базовый класс для всех типов измерения уровня воды

#ifndef ABSTRACTWATERLEVEL_HPP_
#define ABSTRACTWATERLEVEL_HPP_

#include <stdint.h>

class AbstractWaterLever {
public:
    virtual void process() = 0;
    virtual uint8_t getLevel() = 0;
    virtual bool getPermit() = 0;
};

#endif