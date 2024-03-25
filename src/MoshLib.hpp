// Библиотека для робота МОШ (Jamaha)

#pragma once

#include "config.hpp"
#include "hardware/motor.hpp"
#include "hardware/linesensor.hpp"
#include "hardware/distancesensor.hpp"


// ПИНЫ
namespace pin {
// MOTOРЫ
enum MOTOR {
    // ЛЕВЫЙ
    ML_INVERT = false,
    ML_SPEED = 5,
    ML_DIR = 4,
    ML_ENC_A = 2, // ЖЕЛТЫЙ
    ML_ENC_B = 8, // ЗЕЛЁНЫЙ

    // ПРАВЫЙ
    MR_INVERT = true,
    MR_SPEED = 6,
    MR_DIR = 7,
    MR_ENC_A = 3, // ЖЕЛТЫЙ
    MR_ENC_B = 9, // ЗЕЛЁНЫЙ
};

enum LINES { L_SENSOR = A0, R_SENSOR = A1, }; // ДАТЧИКИ ЛИНИИ
enum IR { IR_0 = A2, IR_1 = A3, }; // ИК-ДАТЧИК РАССТОЯНИЯ
enum US { PIN_ECHO = 12, PIN_TRIG = 13, }; // УЗ-ДАТЧИК РАССТОЯНИЯ
}

extern hardware::LineSensor lineL;  // Левый датчик линии
extern hardware::LineSensor lineR;  //Правый датчик линии
extern hardware::IrSensorSharp ir0;  //Левый ИК-датчик расстояния
extern hardware::IrSensorSharp ir1;  // Правый ИК-датчик расстояния
extern hardware::UltraSonic us;  // Ультразвуковой датчик расстояния
extern hardware::MotorEncoder motorL;  // Левый мотор
extern hardware::MotorEncoder motorR;  // Правый мотор


// КОНФИГУРАЦИЯ

/**
 * @brief Указать ЛЕВЫЙ датчик расстояния
 * @param sensor ссылка на датчик
 */
void distSensorL(hardware::DistanceSensor& sensor);

/**
 * @brief Указать ПРАВЫЙ датчик расстояния
 * @param sensor ссылка на датчик
 */
void distSensorR(hardware::DistanceSensor& sensor);

/**
 * @brief Указать ПЕРЕДНИЙ датчик расстояния
 * @param sensor ссылка на датчик
 */
void distSensorF(hardware::DistanceSensor& sensor);


// ФУНКЦИИ ДВИЖЕНИЯ

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
 * @brief Прямолинейное движение до стены (любой датчик)
 * @param sensor ссылка на датчик
 * @param distance расстояние до стены
 * @param speed скорость движения
 */
void goWallFront(hardware::DistanceSensor& sensor, uint8_t distance, uint8_t speed = PARAMS::DEFAULT_SPEED);

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
void goWallBack(hardware::DistanceSensor& sensor, uint8_t distance, uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Прямолинейное движение назад от стены (передний датчик)
 * @param distance расстояния, на котором будет стены
 * @param speed скорость движения
 */
void goWallBack(uint8_t distance, uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Поворот на заданый угол вокруг центра
 * @param a угол поворота ( <0 - поворот против часовой)
 * @param speed скорось поворота
 */
void turnAngle(int16_t a, uint8_t speed = PARAMS::DEFAULT_SPEED);

enum LINE_REGULATORS {
    RELAY_L,
    RELAY_R,
    RELAY_LR,
    PROP,
};

/**
 * @brief Установить регулятор движения по линии по умолчанию
 * @param default_regulator тип регулятора `RELAY_L` | `RELAY_R` | `RELAY_LR` | `PROP`
 */
void lineReg(enum LINE_REGULATORS default_regulator);

/**
 * @brief Движение по линии по времени с произвольным регулятором
 * @param regulator_type тип регулятора `RELAY_L` | `RELAY_R` | `RELAY_LR` | `PROP`
 * @param runtime время движения
 * @param speed скорость
 */
void goLineTime(enum LINE_REGULATORS type, uint32_t runtime, uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Движение по линии с регулятором по умолчанию
 * @param runtime время движения
 * @param speed скорость
 */
void goLineTime(uint32_t runtime, uint8_t speed = PARAMS::DEFAULT_SPEED);

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
}
