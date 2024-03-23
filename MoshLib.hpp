// Библиотека для робота МОШ (Jamaha)
#pragma once

#include "config.hpp"
#include "motor.hpp"
#include "linesensor.hpp"
#include "distancesensor.hpp"


/// @brief Левый датчик линии
LineSensor lineL(cfg::LINE::L_SENSOR, cfg::LINE::L_LINE, cfg::LINE::L_FIELD);

/// @brief Правый датчик линии
LineSensor lineR(cfg::LINE::R_SENSOR, cfg::LINE::R_LINE, cfg::LINE::R_FIELD);

/// @brief Левый ИК-датчик расстояния
IrSensorSharp irL(cfg::IR::L_PIN);

/// @brief Правый ИК-датчик расстояния
IrSensorSharp irR(cfg::IR::R_PIN);

/// @brief Ультразвуковой датчик расстояния
UltraSonic us(cfg::US::PIN_ECHO, cfg::US::PIN_TRIG);

// Переводной макрос из ММ пути в тики энкодера
#define MM2TICKS(mm) ( (int32_t)(mm) * 1000L / (int32_t)(cfg::MOTOR::MM_IN_1000_TICKS) ) // .. где мой constexpr, Ардуино!!? где С++11???


/**
 * @brief Проехать 1000 тиков вперёд прямо
 * @param speed скорость движения
 */
void go_1000_ticks(uint8_t speed = cfg::MOTOR::DEFAULT_SPEED) { motors.setForTicks(speed, 1000, speed, 1000); }


/**
 * @brief Прямолинейное движение
 * @param distance_mm расстояние (отрицательное - едет назад)
 * @param speed скорость движения
 */
void goDirect(int32_t distance_mm, uint8_t speed = cfg::MOTOR::DEFAULT_SPEED) {
    int32_t ticks = MM2TICKS(distance_mm);
    motors.setForTicks(speed, ticks, speed, ticks);
}

/**
 * @brief Поворот на заданый угол вокруг центра
 * @param a угол поворота ( <0 - поворот против часовой)
 * @param speed скорось поворота
 */
void turnAngle(int16_t a, uint8_t speed = cfg::MOTOR::DEFAULT_SPEED) {
    int32_t ticks = MM2TICKS((int32_t)a * cfg::MOTOR::TRACK * M_PI / 360);
    motors.setForTicks(speed, ticks, speed, -ticks);
}

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
        sum_L += lineL.readRaw();
        sum_R += lineR.readRaw();
    }

    ret_L = sum_L / i;
    ret_R = sum_R / i;
}

