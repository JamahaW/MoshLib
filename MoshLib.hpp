// Библиотека для робота МОШ (Jamaha)
#pragma once

#include "pins.hpp"
#include "motor.hpp"

// Переводной макрос из ММ пути в тики энкодера
#define MM2TICKS(mm) ( (int32_t)(mm) * 1000L / (int32_t)(pins::MOTOR::MM_IN_1000_TICKS) ) // .. где мой constexpr, Ардуино!!? где С++11???


/// @brief проехать 1000 тиков (для определения отношения мм/тик)
/// @param speed скорость > 0
void go_1000_ticks(uint8_t speed = pins::MOTOR::DEFAULT_SPEED) { motors.setForTicks(speed, 1000, speed, 1000); }

/// @brief Движение по прямой (вперёд или назад)
/// @param distance_mm дистанция в мм (<0 для движения назад)
/// @param speed скорость > 0
void goDirect(int32_t distance_mm, uint8_t speed = pins::MOTOR::DEFAULT_SPEED) {
    int32_t ticks = MM2TICKS(distance_mm);
    motors.setForTicks(speed, ticks, speed, ticks);
}

/// @brief поворот по часовой стрелке на a градусов вокруг центра
/// @param a угол поворота
void turnAngle(int16_t a, uint8_t speed = pins::MOTOR::DEFAULT_SPEED) {
    int32_t ticks = MM2TICKS((int32_t)a * pins::MOTOR::TRACK * M_PI / 360);
    motors.setForTicks(speed, ticks, speed, -ticks);
}
