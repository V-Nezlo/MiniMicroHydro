//-----------------------------------------------------------------------
//  File        : WaterLevelHandler.cpp
//  Created     : 7.11.2022
//  Modified    : 7.11.2022
//  Author      : V-Nezlo (vlladimirka@gmail.com)
//  Description :

#include "FloatWaterLevel.hpp"

    FloatLevelHandler::FloatLevelHandler(uint32_t aUpdatePeriod, Gpio &aWaterLev1, Gpio *aWaterLev2, Gpio *aWaterLev3, 
		AbstractWaterIndicator *aIndicator, bool aIsFloatLevel, Gpio *aWaterPower):
    waterLev1{aWaterLev1},
    waterLev2{aWaterLev2},
    waterLev3{aWaterLev3},
    waterPower{aWaterPower},
    type{Type::OneSensors},
    nextUpdateTime{millis()},
    updatePeriod{aUpdatePeriod},
    nextBeepTime{millis()},
    isFloatLevel{aIsFloatLevel},
    currentProcents{0},
    permit{false},
    indicator{aIndicator}
{
    if (!isFloatLevel && waterPower == nullptr) {
        printf("Selected electrode level but power pin is not defined");
    }

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
    uint32_t currentTime = millis();
    bool error = false;

    if (currentTime > nextUpdateTime) {
        // Только если имеем дело с электродными датчиками
        if (!isFloatLevel && waterPower != nullptr) {
            waterPower->set(); // Подаем питание на измерительные цепи
        }

        uint8_t procent{0};
        bool water1State = waterLev1.digitalRead(); // Самый низкий датчик, он есть всегда
        bool water2State = waterLev2 != nullptr ? waterLev2->digitalRead() : false; // Датчик повыше
        bool water3State = waterLev3 != nullptr ? waterLev3->digitalRead() : false; // Высокий датчик ваще жесть

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
        // Только если имеем дело с электродными датчиками
        if (!isFloatLevel && waterPower != nullptr) {
            waterPower->reset(); // Убираем питание с измерительных цепей чтобы не вызывать электролиз
        }

        if (indicator != nullptr) {
            indicator->setLevel(procent);
        }

        permit = !error;
        currentProcents = procent;
        nextUpdateTime = currentTime + updatePeriod;
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
