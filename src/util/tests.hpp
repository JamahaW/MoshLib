#pragma once
#include <Arduino.h>


namespace mosh {

// ТЕСТЫ
namespace test {

/**
 * @brief Измерить значения датчиков линий
 * @param ret_L возврат для левого
 * @param ret_R возврат для правого
 * @param timeout время измерения
 */
void lines(uint16_t& ret_L, uint16_t& ret_R, uint32_t timeout = 1000);

/// @brief тест функциональности моторов
void motorsAccel();

/// @brief тест функциональности моторов (ШИМ ВЕРСИЯ)
void motorsPWM();

} // namespace test
} // namespace mosh
