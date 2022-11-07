//  File        : AbstractWaterIndicator.hpp
//  Created     : 23.10.2022
//  Modified    : 23.10.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Базовый класс для всех типов отображения уровня воды

#ifndef ABSTRACTWATERINDICATOR_HPP_
#define ABSTRACTWATERINDICATOR_HPP_

#include <stdint.h>

class AbstractWaterIndicator {
public:
    virtual void setLevel(uint8_t aProcent) = 0;
};

#endif
