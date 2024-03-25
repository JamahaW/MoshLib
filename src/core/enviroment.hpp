#pragma once

#include "hardware/distancesensor.hpp"
#include "hardware/linesensor.hpp"
#include "hardware/motor.hpp"

extern hardware::LineSensor lineL;  // Левый датчик линии
extern hardware::LineSensor lineR;  //Правый датчик линии

extern hardware::NoDistanceSensor no_sensor;
extern hardware::IrSensorSharp ir0;  //Левый ИК-датчик расстояния
extern hardware::IrSensorSharp ir1;  // Правый ИК-датчик расстояния
extern hardware::UltraSonic us;  // Ультразвуковой датчик расстояния

extern hardware::MotorEncoder motorL;  // Левый мотор
extern hardware::MotorEncoder motorR;  // Правый мотор

/// @brief Данные о роботе
struct RobotConfig {
    hardware::DistanceSensor* dist_left = &no_sensor; // используется при движении вдоль стены слева
    hardware::DistanceSensor* dist_right = &no_sensor; // используештся при движении вдоль стены справа
    hardware::DistanceSensor* dist_front = &no_sensor; // используется при движении до объекта спереди
    LINE_REGULATORS line_follow_regulator = LINE_REGULATORS::PROP; // Регулятор движения по линии по умолчанию
};

extern RobotConfig robot;

namespace motors {
void reset();
void spin();
void setSpeeds(int16_t left, int16_t right);
void setForTicks(int8_t speed_L, int32_t ticks_L, int8_t speed_R, int32_t ticks_R);
} // namespace motors