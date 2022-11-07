#include "LedIndicator.hpp"

    LedIndicator::~LedIndicator()
    {

    }

    LedIndicator::LedIndicator(Gpio *aRedLed) :
        type{Type::ONE_LED}
    {
        if (aRedLed != nullptr) {
            ledPins[static_cast<uint8_t>(LedType::RED)] = aRedLed;
        }
    }
 
    LedIndicator::LedIndicator(Gpio *aRedLed, Gpio *aOrangeLed, Gpio *aGreenLed):
        type{Type::THREE_LED}
    {
        if (aRedLed != nullptr && aOrangeLed != nullptr && aGreenLed != nullptr) {
            ledPins[static_cast<uint8_t>(LedType::RED)] = aRedLed;
            ledPins[static_cast<uint8_t>(LedType::ORANGE1)] = aOrangeLed;
            ledPins[static_cast<uint8_t>(LedType::GREEN1)] = aGreenLed;
        }
    }

    LedIndicator::LedIndicator(Gpio *aRedLed, Gpio *aOrange1Led, Gpio *aOrange2Led, Gpio *aGreen1Led, Gpio *aGreen2Led):
        type{Type::FIVE_LED}
    {
        if (aRedLed != nullptr && aOrange1Led != nullptr && aOrange2Led != nullptr && aGreen1Led != nullptr && 
            aGreen2Led != nullptr) {

            ledPins[static_cast<uint8_t>(LedType::RED)] = aRedLed;
            ledPins[static_cast<uint8_t>(LedType::ORANGE1)] = aOrange1Led;
            ledPins[static_cast<uint8_t>(LedType::ORANGE2)] = aOrange2Led;
            ledPins[static_cast<uint8_t>(LedType::GREEN1)] = aGreen1Led;
            ledPins[static_cast<uint8_t>(LedType::GREEN2)] = aGreen2Led;
        }
    }

    void LedIndicator::setLevel(uint8_t aProcent) 
    {
        if (aProcent > 100) {
            printf("Procent over 100 - error!");
            return;
        }

        switch (type) {
            case Type::ONE_LED:
                if (aProcent <= Consts::kRedOneThreshold) {
                    ledPins[static_cast<uint8_t>(LedType::RED)]->set();
                } else {
                    ledPins[static_cast<uint8_t>(LedType::RED)]->reset();
                }
                break;
            case Type::THREE_LED:
                if (aProcent <= Consts::kRedThreeThreshold) {
                    ledPins[static_cast<uint8_t>(LedType::RED)]->set();
                    ledPins[static_cast<uint8_t>(LedType::ORANGE1)]->reset();
                } else {
                    ledPins[static_cast<uint8_t>(LedType::RED)]->reset();
                    ledPins[static_cast<uint8_t>(LedType::ORANGE1)]->set();
                }

                if (aProcent >= Consts::kGreenThreeThreshold) {
                    ledPins[static_cast<uint8_t>(LedType::GREEN1)]->set();
                } else {
                    ledPins[static_cast<uint8_t>(LedType::GREEN1)]->reset();
                }
                break;
            case Type::FIVE_LED:
                if (aProcent <= Consts::kLevelFive0Procent) {
                    ledPins[static_cast<uint8_t>(LedType::RED)]->set();
                    ledPins[static_cast<uint8_t>(LedType::ORANGE1)]->reset();
                } else {
                    ledPins[static_cast<uint8_t>(LedType::RED)]->reset();
                    ledPins[static_cast<uint8_t>(LedType::ORANGE1)]->set();
                }

                if (aProcent >= Consts::kLevelFive1Procent) {
                    ledPins[static_cast<uint8_t>(LedType::ORANGE2)]->set();
                } else {
                    ledPins[static_cast<uint8_t>(LedType::ORANGE2)]->reset();
                }

                if (aProcent >= Consts::kLevelFive2Procent) {
                    ledPins[static_cast<uint8_t>(LedType::GREEN1)]->set();
                } else {
                    ledPins[static_cast<uint8_t>(LedType::GREEN1)]->reset();
                }

                if (aProcent >= Consts::kLevelFive3Procent) {
                    ledPins[static_cast<uint8_t>(LedType::GREEN2)]->set();
                } else {
                    ledPins[static_cast<uint8_t>(LedType::GREEN2)]->reset();
                }
                break;
        }
    }