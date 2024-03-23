// Бакэнд моторов!

#pragma once

#include <Arduino.h>

#include "pins.hpp"


/// @brief Обработчик левого мотора
static void __left_tick();

/// @brief Обработчик правого мотора
static void __right_tick();


enum MOTORSET {
    KP_SPEED = 8, // KP смещение (скорость)
    KP_1_FOLLOW = 4, // 1/KP стремление в точку

    MAX_DELTA_TICKS = 50, // максимальная d_tick при повороте
    SPIN_PERIOD_MS = 32, // период смены целевого положения мотора по d_tick
    HOLD_STOP_TIMEOUT = 1000, // время остановки удержанием позиции
};


/// @brief класс DC-мотор-энкодера, управляемого H-мостом
static class Motor {

private:
    /// @brief Пин скорости
    const uint8_t PIN_SPEED;
    /// @brief Пин направления
    const uint8_t PIN_DIR;
    /// @brief Пин второго канала энкодера
    const uint8_t PIN_ENC_B;

    /// @brief Глобальная инверсия мотора
    const bool INVERTED;

    /// @brief Модуль заданой скорости
    uint8_t speed_set = 0;
    /// @brief регулируемая скорость
    int8_t speed = 0;
    /// @brief таймер смены позиции
    uint32_t next_pos_timer = 0;

public:
    /// @brief следущая целевая позиция
    int32_t next_pos = 0;
    /// @brief цель движения
    int32_t target = 0;
    /// @brief текущее положение
    volatile int32_t position = 0;

    Motor(
        void (*encoder_handler) (void),
        const bool inverted,
        const uint8_t pin_speed,
        const uint8_t pin_dir,
        const uint8_t pin_enc_a,
        const uint8_t pin_enc_b
    ) : PIN_SPEED(pin_speed), PIN_DIR(pin_dir), PIN_ENC_B(pin_enc_b), INVERTED(inverted) {
        pinMode(PIN_SPEED, OUTPUT);
        pinMode(PIN_DIR, OUTPUT);
        pinMode(PIN_ENC_B, INPUT);
        pinMode(pin_enc_a, INPUT);
        attachInterrupt(digitalPinToInterrupt(pin_enc_a), encoder_handler, FALLING);
    }

    /// @brief вызывается только в прерывании
    void enc() { position += (digitalRead(PIN_ENC_B) ^ INVERTED) ? (1) : (-1); }

    /// @brief сброс данных мотора
    void reset() {
        position = 0;
        next_pos = 0;
        speed_set = 0;
        speed = 0;
        next_pos_timer = 0;
        target = 0;
    }

    /// @brief Установить направление (true - нормально, false - реверс)
    /// @param backward 
    void setDir(bool backward) { digitalWrite(PIN_DIR, backward ^ INVERTED); }

    /// @brief Установить шим
    /// @param pwm ШИМ 0..255
    void setPWM(uint8_t pwm) { analogWrite(PIN_SPEED, pwm); }

    /// @brief Установить шим-направление. Отрицательные значения соотвествуют реверсу. ШИМ берётся по модулю и ограничивается.
    /// @param power (-255..255)
    void setDirPWM(int16_t power) {
        setDir(power < 0);
        power = abs(power);
        setPWM(constrain(power, 0, 255));
    }

    /// @brief Установить целевую скорость вращения
    /// @param dtick
    void setSpeed(int8_t dtick) { speed = speed_set = constrain(dtick, -MOTORSET::MAX_DELTA_TICKS, MOTORSET::MAX_DELTA_TICKS); }

    /// @brief вращать мотор (вызывать постоянно)
    void spin() {
        setDirPWM(KP_SPEED * (next_pos - position));
        if (millis() < next_pos_timer) return;
        next_pos_timer = millis() + MOTORSET::SPIN_PERIOD_MS;
        next_pos += speed;
        return;
    }

    /// @brief Стремиться к целевой позиции
    bool follow() {
        spin();
        int32_t u = (target - position) / MOTORSET::KP_1_FOLLOW;
        speed = constrain(u, -abs(speed_set), abs(speed_set));
        return abs(u) > 0;
    }
} leftMotor(
    __left_tick,
    pins::MOTOR::ML_INVERT,
    pins::MOTOR::ML_SPEED,
    pins::MOTOR::ML_DIR,
    pins::MOTOR::ML_ENC_A,
    pins::MOTOR::ML_ENC_B
), rightMotor(
    __right_tick,
    pins::MOTOR::MR_INVERT,
    pins::MOTOR::MR_SPEED,
    pins::MOTOR::MR_DIR,
    pins::MOTOR::MR_ENC_A,
    pins::MOTOR::MR_ENC_B);


static void __left_tick() { leftMotor.enc(); }
static void __right_tick() { rightMotor.enc(); }


struct MotorController {

    /// @brief Отключить ШИМ моторов
    void off() {
        leftMotor.setPWM(0);
        rightMotor.setPWM(0);
    }

    /// @brief Запускать вращение моторов
    void spin() {
        leftMotor.spin();
        rightMotor.spin();
    }

    /// @brief сбросить данные моторов (позиция, цель, таймер, скорость) = 0
    void reset() {
        leftMotor.reset();
        rightMotor.reset();
    }

    /// @brief Установить скорости моторов
    /// @param left 
    /// @param right 
    void setSpeeds(int8_t left, int8_t right) {
        leftMotor.setSpeed(left);
        rightMotor.setSpeed(right);
    }

    /// @brief Включить моторы на время с заданой скоростью
    /// @param speed общая скорость
    /// @param runtime_ms время работы
    void setForTime(int8_t speed, uint32_t runtime_ms) {
        uint32_t end_time = millis() + runtime_ms;
        setSpeeds(speed, speed);
        while (millis() < end_time) spin();
        off();
    }

    /// @brief Остановка моторов удержанием позиции
    void holdingStop() { setForTime(0, MOTORSET::HOLD_STOP_TIMEOUT); }

    /// @brief Включить Асихронное движение моторов на расстояние
    /// @param speed_L скорость левого мотора >= 0
    /// @param ticks_L расстояние для левого колеса (+-)
    /// @param speed_R скорость правого мотора >= 0 
    /// @param ticks_R расстояние для правого колеса (+-)
    void setForTicks(int8_t speed_L, int16_t ticks_L, int8_t speed_R, int16_t ticks_R) {
        reset();
        setSpeeds(speed_L, speed_R);
        leftMotor.target = ticks_L;
        rightMotor.target = ticks_R;

        bool runL = true;
        bool runR = true;

        while (runL || runR) {
            runL = leftMotor.follow();
            runR = rightMotor.follow();
        }

        holdingStop();
    }

} motors;
