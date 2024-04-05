#pragma once

#include <Arduino.h>
#include "hardware/distancesensor.hpp"
#include "core/values.hpp"


namespace mosh {
namespace hardware {

struct RobotConfig {
    /// @brief используется при движении вдоль стены слева
    mosh::hardware::DistanceSensor* dist_left = &__no_sensor;

    /// @brief используештся при движении вдоль стены справа
    mosh::hardware::DistanceSensor* dist_right = &__no_sensor;

    /// @brief используется при движении до объекта спереди
    mosh::hardware::DistanceSensor* dist_front = &__no_sensor;

    /// @brief скорость-тик мотора
    uint8_t default_speed_tick = CONF_DEFAULT::SPEED_TICK;

    /// @brief скорость-ШИМ мотора
    uint8_t default_speed_pwm = CONF_DEFAULT::SPEED_PWM;

    /// @brief колея
    uint8_t track_size = CONF_DEFAULT::TRACK_SIZE_MM;

    /// @brief мм в 1000 тиках
    uint16_t mm_in_1000_ticks = CONF_DEFAULT::MM_IN_1000_TICKS;

    /// @brief время выравнивания
    uint16_t align_duration = CONF_DEFAULT::ALIGN_DURATION;

    /// @brief время удержания позиции
    uint16_t hold_duration = CONF_DEFAULT::HOLD_DURATION;

    /// @brief Перевести мм в тики энкодера
    /// @param mm дистанция в мм
    /// @return дистанция в тиках
    int32_t mm2ticks(int32_t mm);

    // CONFIG

    /// @brief Указать ЛЕВЫЙ датчик расстояния
    /// @param sensor ссылка на датчик
    RobotConfig& distL(mosh::hardware::DistanceSensor& sensor);

    /// @brief Указать ПРАВЫЙ датчик расстояния
    /// @param sensor ссылка на датчик
    RobotConfig& distR(mosh::hardware::DistanceSensor& sensor);

    /// @brief Указать ПЕРЕДНИЙ датчик расстояния
    /// @param sensor ссылка на датчик
    RobotConfig& distF(mosh::hardware::DistanceSensor& sensor);

    /// @brief Задать размер колеи
    /// @param trasksize_mm ширина колеи в мм
    RobotConfig& track(uint8_t trasksize_mm);

    /// @brief Задать скорость в тиках по умолчанию
    /// @param ticks d-tick
    RobotConfig& speedTick(uint8_t ticks);

    /// @brief Задать скорость в ШИМ по умолчанию
    /// @param pwm ШИМ (0..255)
    RobotConfig& speedPWM(uint8_t pwm);

    /// @brief Задать кол-во пути в 1000 тиках энкодера
    /// @param mm_in_1000_ticks 
    RobotConfig& mmInTicks1000(uint16_t mm_in_1000_ticks);
};

} // namespace hardware
} // namespace mosh

/// @brief Конфигурация робота
extern mosh::hardware::RobotConfig conf;

#define SIGN(x) (((x) > 0) ? 1 : -1) // ЗНАК переменной
