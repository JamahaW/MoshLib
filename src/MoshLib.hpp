// Библиотека для робота МОШ (Jamaha)

#pragma once

#include "core/env.hpp"
#include "control/move.hpp"


// ПРОСТОЕ ПЕРЕДВИЖЕНИЕ

/**
 * @brief Движение с заданой скоростью на время
 * @param runtime время работы
 * @param speed_left скорость левого мотора
 * @param speed_right скорость правого мотора
 * @param __hold_at_end удержание моторов в конце движения (для реализации goHold)
 */
void goTime(uint32_t runtime, int8_t speed_left, int8_t speed_right, bool __hold_at_end = true);

/**
 * @brief Движение с заданой скоростью на время
 * @param runtime время работы
 * @param speed скорость
 */
void goTime(uint32_t runtime, int8_t speed = mosh::env::robot.default_speed_tick);

/**
 * @brief Удерживать моторы в текущем положении на время
 * @param timeout время удержания
 */
void goHold(uint32_t timeout = HOLD_DURATION);

/**
 * @brief на число тиков
 * @param speed ШИМ скорость движения
 */
void goTick(int32_t ticks, uint8_t speed = mosh::env::robot.default_speed_pwm);

/**
 * @brief Прямолинейное синхронное движение
 * @param distance_mm расстояние (отрицательное - едет назад)
 * @param mosh::env::robot.default_speed_pwm скорость движения ШИМ
 */
void goDist(int32_t distance_mm, int16_t speed_pwm = mosh::env::robot.default_speed_pwm);

/**
 * @brief Прямолинейное синхронное движение до перекрёстка
 * @param crosses кол-во перекрёстков
 * @param found_line искомое состояние
 * @param mosh::env::robot.default_speed_pwmDIR скорость (отрицательная - движение назад)
 */
void goCross(uint8_t crosses = 1, bool found_line = false, int16_t speed_pwmDIR = mosh::env::robot.default_speed_pwm);

/**
 * @brief синхронный Поворот на заданый угол вокруг центра
 * @param a угол поворота ( <0 - поворот против часовой)
 * @param speed ШИМ скорось поворота
 */
void turnAngle(int16_t a, int16_t speed = mosh::env::robot.default_speed_pwm);


/**
 * @brief Выровнять робота по линии
 * @param duration время выравнивания
 */
void alignLine(uint32_t duration = mosh::env::robot.align_duration);


/**
 * @brief повернуться вокруг центра влево пересекая линии
 * @param crosses кол-во линий
 * @param speed ШИМ скорость
 */
void turnLineL(int8_t crosses = 1, int16_t speed = mosh::env::robot.default_speed_pwm);

/**
 * @brief повернуться вокруг центра направо по перекрёсткам
 * @param crosses кол-во линий
 * @param speed ШИМ скорость
 */
void turnLineR(int8_t crosses = 1, int16_t speed = mosh::env::robot.default_speed_pwm);

// ДВИЖЕНИЕ ДО-ПО-ВДОЛЬ стены

/**
 * @brief Прямолинейное синхронное движение до стены (любой датчик)
 * @param sensor ссылка на датчик
 * @param distance расстояние до стены
 * @param speed ШИМ скорость движения
 */
void wallFront(mosh::hardware::DistanceSensor& sensor, uint8_t distance, uint8_t speed = mosh::env::robot.default_speed_pwm);

/**
 * @brief Прямолинейное синхронное движение до стены (передний датчик)
 * @param distance расстояние до стены
 * @param speed ШИМ скорость движения
 */
void wallFront(uint8_t distance, uint8_t speed = mosh::env::robot.default_speed_pwm);

/**
 * @brief Прямолинейное синхронное движение назад от стены (любой датчик)
 * @param sensor ссылка на датчик
 * @param distance расстояние до стены
 * @param speed ШИМ скорость движения
 */
void wallBack(mosh::hardware::DistanceSensor& sensor, uint8_t distance, int16_t speed = mosh::env::robot.default_speed_pwm);

/**
 * @brief Прямолинейное синхронное движение назад от стены (передний датчик)
 * @param distance расстояния, на котором будет стены
 * @param speed ШИМ скорость движения
 */
void wallBack(uint8_t distance, uint8_t speed = mosh::env::robot.default_speed_pwm);

/**
 * @brief Движение вдоль стены по ЛЕВОМУ датчику
 * @param distance целевое расстояние в см
 * @param runtime время движения
 * @param speed скорость
 */
void wallTimeL(uint8_t distance, uint32_t runtime, uint8_t speed = mosh::env::robot.default_speed_tick);

/**
 * @brief Движение вдоль стены по ПРАВОМУ датчику
 * @param distance целевое расстояние в см
 * @param runtime время движения
 * @param speed скорость
 */
void wallTimeR(uint8_t distance, uint32_t runtime, uint8_t speed = mosh::env::robot.default_speed_tick);


// ДВИЖЕНИЕ ПО ЛИНИИ

// /// @brief движение по линии с регулятором по умолчанию
// /// @param speed скорость
// void lineTime(uint32_t runtime, uint8_t speed = DEFAULT_SPEED);

/// @brief движение по линии по левому датчику
/// @param speed скорость
void lineTimeL(uint32_t runtime, uint8_t speed = mosh::env::robot.default_speed_tick);

/// @brief движение по линии по правому датчику
/// @param speed скорость
void lineTimeR(uint32_t runtime, uint8_t speed = mosh::env::robot.default_speed_tick);

/// @brief движение по линии по двум датчикам
/// @param speed скорость
void lineTimeLR(uint32_t runtime, uint8_t speed = mosh::env::robot.default_speed_tick);

/// @brief движение по линии по P-регулятору
/// @param speed скорость
void lineTimeP(uint32_t runtime, uint8_t speed = mosh::env::robot.default_speed_tick);


/**
 * @brief Движение по линии до перекрёстка P-регулятор
 * @param crosses на каком перекрёстке остановиться
 * @param speed скорость ТИК
 */
void lineCrossP(uint8_t crosses = 1, uint8_t speed = mosh::env::robot.default_speed_tick);

/// @brief Движение по линии на расстояние P-регулятор
/// @param distance Расстояние движения
/// @param speed скорость ТИК
void lineDistP(int32_t distance, uint8_t speed = mosh::env::robot.default_speed_tick);


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
