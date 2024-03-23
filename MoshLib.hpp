// Библиотека для робота МОШ (Jamaha)
#pragma once

#include "config.hpp"
#include "motor.hpp"
#include "linesensor.hpp"


// Переводной макрос из ММ пути в тики энкодера
#define MM2TICKS(mm) ( (int32_t)(mm) * 1000L / (int32_t)(cfg::MOTOR::MM_IN_1000_TICKS) ) // .. где мой constexpr, Ардуино!!? где С++11???


/// @brief проехать 1000 тиков (для определения отношения мм/тик)
/// @param speed скорость > 0
void go_1000_ticks(uint8_t speed = cfg::MOTOR::DEFAULT_SPEED) { motors.setForTicks(speed, 1000, speed, 1000); }

/// @brief Движение по прямой (вперёд или назад)
/// @param distance_mm дистанция в мм (<0 для движения назад)
/// @param speed скорость > 0
void goDirect(int32_t distance_mm, uint8_t speed = cfg::MOTOR::DEFAULT_SPEED) {
    int32_t ticks = MM2TICKS(distance_mm);
    motors.setForTicks(speed, ticks, speed, ticks);
}

/// @brief поворот по часовой стрелке на a градусов вокруг центра
/// @param a угол поворота
void turnAngle(int16_t a, uint8_t speed = cfg::MOTOR::DEFAULT_SPEED) {
    int32_t ticks = MM2TICKS((int32_t)a * cfg::MOTOR::TRACK * M_PI / 360);
    motors.setForTicks(speed, ticks, speed, -ticks);
}

/**
 * @brief определить уровень линии на Левом датчике
 * @return нахождение на линии (0 - 100) %
 */
int8_t readLineL() { return leftLine.read(); }

/**
 * @brief определить уровень линии на Правом датчике
 * @return нахождение на линии (0 - 100) %
 */
int8_t readLineR() { return rightLine.read(); }

/**
 * @brief Измерить значения датчиков линий
 * @param ret_L возврат для левого
 * @param ret_R возврат для правого
 * @param timeout время измерения
 */
void test_lineSensors(uint16_t& ret_L, uint16_t& ret_R, uint32_t timeout = 1000) {
    uint16_t i;
    uint32_t sum_L = 0, sum_R = 0;
    timeout += millis();

    for (i = 0; millis() < timeout; i++) {
        sum_L += leftLine.readRaw();
        sum_R += rightLine.readRaw();
    }

    ret_L = sum_L / i;
    ret_R = sum_R / i;
}
