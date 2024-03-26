// Библиотека для робота МОШ (Jamaha)

#pragma once

#include "core\config.hpp"
#include "core/enviroment.hpp"


// КОНФИГУРИРОВАНИЕ

/**
 * @brief Указать ЛЕВЫЙ датчик расстояния
 * @param sensor ссылка на датчик
 */
void setDistL(mosh::hardware::DistanceSensor& sensor);

/**
 * @brief Указать ПРАВЫЙ датчик расстояния
 * @param sensor ссылка на датчик
 */
void setDistR(mosh::hardware::DistanceSensor& sensor);

/**
 * @brief Указать ПЕРЕДНИЙ датчик расстояния
 * @param sensor ссылка на датчик
 */
void setDistF(mosh::hardware::DistanceSensor& sensor);

/**
 * @brief Установить регулятор движения по линии по умолчанию
 * @param default_regulator тип регулятора `RELAY_L` | `RELAY_R` | `RELAY_LR` | `PROP`
 */
void setLineReg(enum LINE_REGULATORS default_regulator);


// ПРОСТОЕ ПЕРЕДВИЖЕНИЕ

/**
 * @brief Движение с заданой скоростью на время
 * @param runtime время работы
 * @param speed_left скорость левого мотора
 * @param speed_right скорость правого мотора
 * @param __hold_at_end для реализации goHold
 */
void goTime(uint32_t runtime, int8_t speed_left, int8_t speed_right, bool __hold_at_end = true);

/**
 * @brief Движение с заданой скоростью на время
 * @param runtime время работы
 * @param speed скорость
 */
void goTime(uint32_t runtime, int8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Удерживать моторы в текущем положении на время
 * @param timeout время удержания
 */
void goHold(uint32_t timeout = PARAMS::HOLD_STOP_TIMEOUT);

/**
 * @brief на число тиков
 * @param speed скорость движения
 */
void goTick(int32_t ticks, uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Прямолинейное движение
 * @param distance_mm расстояние (отрицательное - едет назад)
 * @param speed скорость движения
 */
void goDist(int32_t distance_mm, uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Поворот на заданый угол вокруг центра
 * @param a угол поворота ( <0 - поворот против часовой)
 * @param speed скорось поворота
 */
void turnAngle(int16_t a, uint8_t speed = PARAMS::DEFAULT_SPEED);

// ДВИЖЕНИЕ ДО-ПО-ВДОЛЬ стены

/**
 * @brief Прямолинейное движение до стены (любой датчик)
 * @param sensor ссылка на датчик
 * @param distance расстояние до стены
 * @param speed скорость движения
 */
void goWallFront(mosh::hardware::DistanceSensor& sensor, uint8_t distance, uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Прямолинейное движение до стены (передний датчик)
 * @param distance расстояние до стены
 * @param speed скорость движения
 */
void goWallFront(uint8_t distance, uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Прямолинейное движение назад от стены (любой датчик)
 * @param sensor ссылка на датчик
 * @param distance расстояние до стены
 * @param speed скорость движения
 */
void goWallBack(mosh::hardware::DistanceSensor& sensor, uint8_t distance, uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Прямолинейное движение назад от стены (передний датчик)
 * @param distance расстояния, на котором будет стены
 * @param speed скорость движения
 */
void goWallBack(uint8_t distance, uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Движение вдоль стены по ЛЕВОМУ датчику
 * @param distance целевое расстояние в см
 * @param runtime время движения
 * @param speed скорость
 */
void goLwallTime(uint8_t distance, uint32_t runtime, uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Движение вдоль стены по ПРАВОМУ датчику
 * @param distance целевое расстояние в см
 * @param runtime время движения
 * @param speed скорость
 */
void goRwallTime(uint8_t distance, uint32_t runtime, uint8_t speed = PARAMS::DEFAULT_SPEED);


// ДВИЖЕНИЕ ПО ЛИНИИ

/**
 * @brief Движение по линии по времени с произвольным регулятором
 * @param regulator_type тип регулятора `RELAY_L` | `RELAY_R` | `RELAY_LR` | `PROP`
 * @param runtime время движения
 * @param speed скорость
 */
void lineTime(LINE_REGULATORS type, uint32_t runtime, uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Движение по линии с регулятором по умолчанию
 * @param runtime время движения
 * @param speed скорость
 */
void lineTime(uint32_t runtime, uint8_t speed = PARAMS::DEFAULT_SPEED);


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
