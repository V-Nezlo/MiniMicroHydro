//-----------------------------------------------------------------------
//  File        : LedIndicator.hpp
//  Created     : 23.10.2022
//  Modified    : 7.11.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description : Отдельный класс для работы с LED

/* Небольшое пояснение - Подразумевается использование нескольких комбинаций:
 * 5 светодиодов - красный - ошибка, два оранжевых, два зеленых
 * 3 светодиода - красный, желтый, зеленый
 * 1 светодиод - только красный
 * Соответственно для каждого варианта свой конструктор
*/

#ifndef LEDINDICATOR_HPP
#define LEDINDICATOR_HPP

#include "AbstractWaterIndicator.hpp"
#include "Constants.hpp"
#include "GpioWrapper.hpp"

class LedIndicator : public AbstractWaterIndicator {

    enum class LedType : uint8_t {
        RED = 0,
        ORANGE1 = 1,
        ORANGE2 = 2,
        GREEN1 = 3,
        GREEN2 = 4,
    };

    enum class Type {
        ONE_LED,
        THREE_LED,
        FIVE_LED
    };

public:

    LedIndicator(Gpio *aRedLed);
    LedIndicator(Gpio *aRedLed, Gpio *aOrangeLed, Gpio *aGreenLed);
    LedIndicator(Gpio *aRedLed, Gpio *aOrange1Led, Gpio *aOrange2Led, Gpio *aGreen1Led, Gpio *aGreen2Led);
    LedIndicator(const LedIndicator &) = delete;
    LedIndicator operator=(const LedIndicator &) = delete;

    virtual ~LedIndicator();

    void setLevel(uint8_t aProcent) override;

private:
    Type type;
    Gpio *ledPins[5];
};

#endif