#pragma once

#include <Arduino.h>
#include "hardware/distancesensor.hpp"


namespace mosh {
namespace hardware {

struct RobotConfig {
    /// @brief используется при движении вдоль стены слева
    mosh::hardware::DistanceSensor* dist_left;

    /// @brief используештся при движении вдоль стены справа
    mosh::hardware::DistanceSensor* dist_right;

    /// @brief используется при движении до объекта спереди
    mosh::hardware::DistanceSensor* dist_front;

    /// @brief порог серого значения
    uint8_t __line_gray_percent;

    /// @brief скорость-тик мотора
    uint8_t __default_speed_tick;

    /// @brief скорость-ШИМ мотора
    uint8_t __default_speed_pwm;

    /// @brief колея
    uint8_t __track_size;

    /// @brief мм в 1000 тиках
    uint16_t __mm_in_1000_ticks;

    /// @brief время выравнивания
    uint16_t __align_duration;

    /// @brief время удержания позиции
    uint16_t __hold_duration;

    /// @brief Конфигурация системы робота
    RobotConfig();

    /// @brief Перевести мм в тики энкодера
    /// @param mm дистанция в мм
    /// @return дистанция в тиках
    int32_t mm2ticks(int32_t mm);

    // CONFIG

    /// @brief Вернуть значения конфигурации к исходным
    /// @return сссылка на себя
    RobotConfig& stock();

    /// @brief Указать ЛЕВЫЙ датчик расстояния
    /// @param sensor ссылка на датчик расстояния
    /// @return сссылка на себя
    RobotConfig& distL(mosh::hardware::DistanceSensor& sensor);

    /// @brief Указать ПРАВЫЙ датчик расстояния
    /// @param sensor ссылка на датчик расстояния
    /// @return сссылка на себя
    RobotConfig& distR(mosh::hardware::DistanceSensor& sensor);

    /// @brief Указать ПЕРЕДНИЙ датчик расстояния
    /// @param sensor ссылка на датчик расстояния
    /// @return сссылка на себя
    RobotConfig& distF(mosh::hardware::DistanceSensor& sensor);

    /// @brief Задать размер колеи
    /// @param trasksize_mm ширина колеи в мм
    /// @return сссылка на себя
    RobotConfig& track(uint8_t trasksize_mm);

    /// @brief Задать скорость в тиках по умолчанию
    /// @param ticks d-tick
    /// @return сссылка на себя
    RobotConfig& speedTick(uint8_t ticks);

    /// @brief Задать скорость в ШИМ по умолчанию
    /// @param pwm ШИМ (0..255)
    /// @return сссылка на себя
    RobotConfig& speedPWM(uint8_t pwm);

    /// @brief Задать кол-во пути в 1000 тиках энкодера
    /// @param mm_in_1000_ticks 
    /// @return сссылка на себя
    RobotConfig& mmInTicks1000(uint16_t mm_in_1000_ticks);

    /// @brief Установить длительность выравнивания по линии
    /// @param duration длительность (мс)
    /// @return сссылка на себя
    RobotConfig& alignTime(uint16_t duration);

    /// @brief Установить длительность удерживания позиции
    /// @param duration длительность (мс)
    /// @return сссылка на себя
    RobotConfig& holdTime(uint16_t duration);

    /// @brief Задать значение чувствительности к линии
    /// @param percent процент чувствительности к значению линии
    /// @return ссылка на себя
    RobotConfig& lineGray(uint8_t percent);
};

} // namespace hardware
} // namespace mosh

/// @brief Конфигурация робота
extern mosh::hardware::RobotConfig conf;

#define SIGN(x) (((x) > 0) ? 1 : -1) // ЗНАК переменной
