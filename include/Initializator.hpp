//-----------------------------------------------------------------------
//  File        : Initializator.hpp
//  Created     : 8.10.2022
//  Modified    : 7.11.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Свистелки - перделки, не более

#ifndef INITIALIZATOR
#define INITIALIZATOR

#include <Arduino.h>
#include "GpioWrapper.hpp"

class Initializator {
public:
    Initializator(Gpio *aGreen1, Gpio *aGreen2, Gpio *aOrange1, Gpio *aOrange2, Gpio *aRed, Gpio *aBuzzer, Gpio *aBlue):
        green1{aGreen1},
        green2{aGreen2},
        orange1{aOrange1},
        orange2{aOrange2},
        red{aRed},
        buzzer{aBuzzer},
        blue{aBlue}
    {
        initialProcedure();
    }

    virtual ~Initializator()
    {
        macro1(green1, false);
        macro1(green2, false);
        macro1(orange1, false);
        macro1(orange2, false);
        macro1(red, false);
        macro1(buzzer, false);
        macro1(blue, false);
    }

    void initialProcedure()
    {
        macro1(buzzer, true);
        delay(100);
        macro1(buzzer, false);
        delay(300);
        macro2(red, 200);
        macro2(orange2, 200);
        macro2(orange1, 200);
        macro2(green2, 200);
        macro2(green1, 200);
        delay(500);
        macro2(blue, 500);
    }
    
private:
    Gpio *green1;
    Gpio *green2;
    Gpio *orange1;
    Gpio *orange2;
    Gpio *red;
    Gpio *buzzer;
    Gpio *blue;

    inline void macro1(Gpio *aGpio, bool aState) 
    {
        if (aGpio != nullptr) {
            aGpio->setState(aState);
        }
    }

    inline void macro2(Gpio *aGpio, uint16_t aDelay)
    {
        macro1(buzzer, true);
        macro1(aGpio, true);
        delay(aDelay);
        macro1(buzzer, false);
    }
};

#endif