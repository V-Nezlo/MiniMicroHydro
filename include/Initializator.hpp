//-----------------------------------------------------------------------
//  File        : Initializator.hpp
//  Created     : 8.10.2022
//  Modified    : 8.10.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Свистелки - перделки, не более

#ifndef INITIALIZATOR
#define INITIALIZATOR

#include <Arduino.h>
#include "GpioWrapper.hpp"

class Initializator {
public:
    Initializator(Gpio &aGreen1, Gpio &aGreen2, Gpio &aOrange, Gpio &aRed, Gpio &aBuzzer, Gpio &aBlue):
        green1{aGreen1},
        green2{aGreen2},
        orange{aOrange},
        red{aRed},
        buzzer{aBuzzer},
        blue{aBlue}
    {
        initialProcedure();
    }

    virtual ~Initializator()
    {
        green1.reset();
        green2.reset();
        orange.reset();
        red.reset();
        blue.reset();
    }

    void initialProcedure()
    {
        buzzer.set();
        delay(100);
        buzzer.reset();

        delay(300);

        buzzer.set();
        red.set();
        delay(200);
        buzzer.reset();

        buzzer.set();
        orange.set();
        delay(200);
        buzzer.reset();

        buzzer.set();
        green2.set();
        delay(200);
        buzzer.reset();

        buzzer.set();
        green1.set();
        delay(200);
        buzzer.reset();

        delay(500);

        buzzer.set();
        blue.set();
        delay(400);
        buzzer.reset();
    }
    
private:
    Gpio &green1;
    Gpio &green2;
    Gpio &orange;
    Gpio &red;
    Gpio &buzzer;
    Gpio &blue;
};

#endif