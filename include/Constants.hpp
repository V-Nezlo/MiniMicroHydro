#ifndef CONSTANTS
#define CONSTANTS

#include <stdint.h>

/* Выношу константы, которые в любом случае придется править руками в более доступную
 * часть кода
*/
namespace Consts
{
    // Пороговые значения для отключения при 5 пинах
	static constexpr uint8_t kLevelFive3Procent{80};
	static constexpr uint8_t kLevelFive2Procent{60};
	static constexpr uint8_t kLevelFive1Procent{40};
	static constexpr uint8_t kLevelFive0Procent{20};

    // Пороговые значения для отключения при 3 пинах
    static constexpr uint8_t kOrangeThreeThreshold{40};
    static constexpr uint8_t kGreenThreeThreshold{80};
    static constexpr uint8_t kRedThreeThreshold{20};

    // Пороговое значение для одного аварийного пина
    static constexpr uint8_t kRedOneThreshold{20};
    static constexpr uint8_t kFullOnePlaceholder{80}; // Плейсхолдер для уровня выше первого уровня
}

#endif