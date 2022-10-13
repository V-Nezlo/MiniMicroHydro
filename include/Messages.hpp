//-----------------------------------------------------------------------
//  File        : Messages.hpp
//  Created     : 6.10.2022
//  Modified    : 8.10.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Структуры для общения между модулями

#ifndef MESSAGES
#define MESSAGES

#include <stdint.h>

namespace Messages
{
    struct HydroParams {
        uint32_t pumpOnTime;
        uint32_t pumpOffTime;
    };
}

#endif