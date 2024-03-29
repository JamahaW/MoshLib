#pragma once

#include "hardware/distancesensor.hpp"
#include "hardware/linesensor.hpp"
#include "hardware/motorencoder.hpp"


extern mosh::hardware::LineSensor lineL;  // Левый датчик линии
extern mosh::hardware::LineSensor lineR;  //Правый датчик линии

extern mosh::hardware::NoDistanceSensor no_sensor;
extern mosh::hardware::IrSensorSharp ir0;  //Левый ИК-датчик расстояния
extern mosh::hardware::IrSensorSharp ir1;  // Правый ИК-датчик расстояния
extern mosh::hardware::UltraSonic us;  // Ультразвуковой датчик расстояния

extern mosh::hardware::MotorEncoder motorL;  // Левый мотор
extern mosh::hardware::MotorEncoder motorR;  // Правый мотор

/// @brief Данные о роботе
struct RobotConfig {
    mosh::hardware::DistanceSensor* dist_left = &no_sensor; // используется при движении вдоль стены слева
    mosh::hardware::DistanceSensor* dist_right = &no_sensor; // используештся при движении вдоль стены справа
    mosh::hardware::DistanceSensor* dist_front = &no_sensor; // используется при движении до объекта спереди
    // LINE_REGULATORS line_follow_regulator = LINE_REGULATORS::PROP; // TODO Регулятор движения по линии по умолчанию 
};

extern RobotConfig robot;

namespace motors {
void reset();
void spin();
void setSpeeds(int16_t left, int16_t right);
void setForTicks(int8_t speed_L, int32_t ticks_L, int8_t speed_R, int32_t ticks_R);
} // namespace motors