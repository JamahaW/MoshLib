// Библиотека для робота МОШ (Jamaha)


#pragma once

#include "config.hpp"
#include "hardware/motor.hpp"
#include "hardware/linesensor.hpp"
#include "hardware/distancesensor.hpp"


// ПИНЫ
namespace pin
{
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

    // ДАТЧИКИ ЛИНИИ
    enum LINES {
        L_SENSOR = A0,
        R_SENSOR = A1,
    };

    // ИК-ДАТЧИК РАССТОЯНИЯ
    enum IR {
        L_PIN = A2,
        R_PIN = A3,
    };

    // УЗ-ДАТЧИК РАССТОЯНИЯ
    enum US {
        PIN_ECHO = 12,
        PIN_TRIG = 13,
    };
}

/// @brief Левый датчик линии
hardware::LineSensor lineL(pin::L_SENSOR, PARAMS::L_LINE, PARAMS::L_FIELD);

/// @brief Правый датчик линии
hardware::LineSensor lineR(pin::R_SENSOR, PARAMS::R_LINE, PARAMS::R_FIELD);

/// @brief Левый ИК-датчик расстояния
hardware::IrSensorSharp irL(pin::L_PIN);

/// @brief Правый ИК-датчик расстояния
hardware::IrSensorSharp irR(pin::R_PIN);

/// @brief Ультразвуковой датчик расстояния
hardware::UltraSonic us(pin::PIN_ECHO, pin::PIN_TRIG);

/// @brief Левый мотор
hardware::MotorEncoder motorL(hardware::__l_int, pin::ML_INVERT, pin::ML_SPEED, pin::ML_DIR, pin::ML_ENC_A, pin::ML_ENC_B);

/// @brief Праавый мотор
hardware::MotorEncoder motorR(hardware::__r_int, pin::MR_INVERT, pin::MR_SPEED, pin::MR_DIR, pin::MR_ENC_A, pin::MR_ENC_B);

void hardware::__l_int() { motorL.enc(); }
void hardware::__r_int() { motorR.enc(); }


/**
 * @brief Удерживать моторы в текущем положении на время
 * @param timeout время удержания
 */
void goHold(uint32_t timeout = PARAMS::HOLD_STOP_TIMEOUT);

// управление моторами
namespace motors
{
    void reset() {
        motorL.reset();
        motorR.reset();
    }

    void spin() {
        motorL.spin();
        motorR.spin();
    }

    void setSpeeds(int8_t left, int8_t right) {
        motorL.setSpeed(left);
        motorR.setSpeed(right);
    }

    void setForTime(int8_t speed, uint32_t runtime_ms) {
        uint32_t end_time = millis() + runtime_ms;
        setSpeeds(speed, speed);
        while (millis() < end_time) spin();
        motorL.setPWM(0);
        motorR.setPWM(0);
    }

    void setForTicks(int8_t speed_L, int32_t ticks_L, int8_t speed_R, int32_t ticks_R) {
        reset();
        setSpeeds(speed_L, speed_R);
        motorL.target = ticks_L;
        motorR.target = ticks_R;

        bool runL = true;
        bool runR = true;

        while (runL || runR) {
            runL = motorL.follow();
            runR = motorR.follow();
        }

        goHold();
    }
} // namespace motors

void goHold(uint32_t timeout) { motors::setForTime(0, timeout); }


/**
 * @brief Проехать 1000 тиков вперёд прямо
 * @param speed скорость движения
 */
void go_1000_ticks(uint8_t speed = PARAMS::DEFAULT_SPEED) { motors::setForTicks(speed, 1000, speed, 1000); }


// Переводной макрос из ММ пути в тики энкодера
#define MM2TICKS(mm) ( (int32_t)(mm) * 1000L / (int32_t)(PARAMS::MM_IN_1000_TICKS) ) // .. где мой constexpr, Ардуино!!? где С++11???


/**
 * @brief Прямолинейное движение
 * @param distance_mm расстояние (отрицательное - едет назад)
 * @param speed скорость движения
 */
void goDirect(int32_t distance_mm, uint8_t speed = PARAMS::DEFAULT_SPEED) {
    int32_t ticks = MM2TICKS(distance_mm);
    motors::setForTicks(speed, ticks, speed, ticks);
}

/**
 * @brief Поворот на заданый угол вокруг центра
 * @param a угол поворота ( <0 - поворот против часовой)
 * @param speed скорось поворота
 */
void turnAngle(int16_t a, uint8_t speed = PARAMS::DEFAULT_SPEED) {
    int32_t ticks = MM2TICKS((int32_t)a * PARAMS::TRACK * M_PI / 360.0);
    motors::setForTicks(speed, ticks, speed, -ticks);
}

// ТЕСТ КОМПОНЕНТОВ
namespace test {

    /**
     * @brief Измерить значения датчиков линий
     * @param ret_L возврат для левого
     * @param ret_R возврат для правого
     * @param timeout время измерения
     */
    void lines(uint16_t& ret_L, uint16_t& ret_R, uint32_t timeout = 1000) {
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

} // namespace test