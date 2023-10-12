//-----------------------------------------------------------------------
//  File        : WaterLevelHandler.cpp
//  Created     : 7.11.2022
//  Modified    : 12.10.2023
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description :

#include "FloatWaterLevel.hpp"

    FloatLevelHandler::FloatLevelHandler(uint32_t aUpdatePeriod, bool aInversion, Gpio &aWaterLev1, Gpio *aWaterLev2, Gpio *aWaterLev3, 
		AbstractWaterIndicator *aIndicator, Gpio *aBeeper):
    waterLev1{aWaterLev1},
    waterLev2{aWaterLev2},
    waterLev3{aWaterLev3},
    beeper{aBeeper},
    type{Type::OneSensors},
    previousUpdateTime{TimeWrapper::milliseconds()},
    updatePeriod{aUpdatePeriod},
    previousBeepTime{TimeWrapper::milliseconds()},
    currentProcents{0},
    permit{false},
    beepState{false},
    inversion{aInversion},
    indicator{aIndicator}
{
    if (aWaterLev3 != nullptr) {
        type = Type::ThreeSensors;
    } else if (aWaterLev2 != nullptr) {
        type = Type::TwoSensors;
    } else {
        type = Type::OneSensors;
    }
}

FloatLevelHandler::~FloatLevelHandler()
{

}

void FloatLevelHandler::process()
{
    uint32_t currentTime = TimeWrapper::milliseconds();
    bool error = false;

    // Защита от переполнения
    if (currentTime < previousUpdateTime) {
        previousUpdateTime = 0;
    }
    if (currentTime < previousBeepTime) {
        previousBeepTime = 0;
    }

    if (currentTime > previousUpdateTime + updatePeriod) {
        previousUpdateTime = currentTime;

        uint8_t procent{0};
        bool water1State = waterLev1.digitalRead(); // Самый низкий датчик, он есть всегда
        bool water2State = waterLev2 != nullptr ? waterLev2->digitalRead() : false; // Датчик повыше
        bool water3State = waterLev3 != nullptr ? waterLev3->digitalRead() : false; // Высокий датчик ваще жесть

        if (inversion) {
            water1State = !water1State;
            water2State = !water2State;
            water3State = !water3State;
        }

        switch (type) {
            case Type::ThreeSensors:
                if (water3State && water2State && water1State) {
                    procent = Consts::kLevelFive3Procent;
                } else if (water2State && water1State) {
                    procent = Consts::kLevelFive2Procent;
                } else if (water1State) {
                    procent = Consts::kLevelFive1Procent;
                } else {
                    procent = Consts::kLevelFive0Procent;
                    error = true;
                }
                break;
            case Type::TwoSensors:
                if (water1State && water2State) {
                    procent = Consts::kGreenThreeThreshold;
                } else if (water1State) {
                    procent = Consts::kOrangeThreeThreshold;
                } else {
                    procent = Consts::kRedThreeThreshold;
                    error = true;
                }
                break;
            case Type::OneSensors:
                if (water1State) {
                    procent = Consts::kFullOnePlaceholder;
                } else {
                    procent = Consts::kRedOneThreshold;
                    error = true;
                }
                break;
        }

        if (indicator != nullptr) {
            indicator->setLevel(procent);
        }

        permit = !error;
        currentProcents = procent;
    }

    // Яростно кричим зуммером если вода на исходе
    if (error && beeper != nullptr && (currentTime > previousBeepTime + kBeepOnTime
        || currentTime > previousBeepTime + kBeepOffTime)) {
        previousBeepTime = currentTime;
        if (beepState && currentTime > previousBeepTime + kBeepOnTime) {
            beepState = false;
            beeper->setState(false);
        } else if (!beepState && currentTime > previousBeepTime + kBeepOffTime) {
            beepState = true;
            beeper->setState(true);
        }
    }
}

uint8_t FloatLevelHandler::getLevel()
{
    return currentProcents;
}

bool FloatLevelHandler::getPermit()
{
    return permit;
}
