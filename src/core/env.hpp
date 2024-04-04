#pragma once

#include "hardware/distancesensor.hpp"
#include "hardware/linesensor.hpp"
#include "hardware/motorencoder.hpp"


/// @brief Левый датчик линии
extern mosh::hardware::LineSensor lineL;

/// @brief Правый датчик линии
extern mosh::hardware::LineSensor lineR;


/// @brief Заглушка
extern mosh::hardware::NoDistanceSensor __no_sensor;

/// @brief Левый ИК-датчик расстояния
extern mosh::hardware::IrSensorSharp ir0;

/// @brief Правый ИК-датчик расстояния
extern mosh::hardware::IrSensorSharp ir1;

/// @brief Ультразвуковой датчик расстояния
extern mosh::hardware::UltraSonic us;


/// @brief Левый мотор
extern mosh::hardware::MotorEncoder motorL;

/// @brief Правый мотор
extern mosh::hardware::MotorEncoder motorR;


namespace mosh {
namespace env {

/// @brief контроллер робота
struct RobotController {
    /// @brief используется при движении вдоль стены слева
    mosh::hardware::DistanceSensor* dist_left = &__no_sensor;

    /// @brief используештся при движении вдоль стены справа
    mosh::hardware::DistanceSensor* dist_right = &__no_sensor;

    /// @brief используется при движении до объекта спереди
    mosh::hardware::DistanceSensor* dist_front = &__no_sensor;

    /// @brief Сброс данных моторов
    void motorsReset();
    
    /// @brief Обновить поворот моторов по энкодеру
    void motorsSpin();

    /// @brief Задать целевые значения скоростей моторов
    /// @param left скорость-тик левого мотора
    /// @param right скорость-тик правого мотора
    void motorsSpeed(int16_t left, int16_t right);

    /// @brief Установить Направление-ШИМ для моторов
    /// @param dirpwm_left направление-ШИМ левого мотора
    /// @param dirpwm_right направление-ШИМ правого мотора
    void motorsDirPWM(int16_t dirpwm_left, int16_t dirpwm_right);
};

extern RobotController robot;

} //namespace env
} // namespace mosh


/// @brief Указать ЛЕВЫЙ датчик расстояния
/// @param sensor ссылка на датчик
void setDistL(mosh::hardware::DistanceSensor& sensor);


/// @brief Указать ПРАВЫЙ датчик расстояния
/// @param sensor ссылка на датчик
void setDistR(mosh::hardware::DistanceSensor& sensor);


/// @brief Указать ПЕРЕДНИЙ датчик расстояния
/// @param sensor ссылка на датчик
void setDistF(mosh::hardware::DistanceSensor& sensor);
