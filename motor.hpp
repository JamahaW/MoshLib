#pragma once

#include <Arduino.h>
#include "config.hpp"

namespace sys
{
    /// @brief Обработчик левого мотора
    void __l_int();

    /// @brief Обработчик правого мотора
    void __r_int();


    /// @brief класс DC-мотор-энкодера, управляемого H-мостом
    class Motor {

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

        /// @brief вызывается исключительно в прерывании
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
        void setSpeed(int8_t dtick) { speed = speed_set = constrain(dtick, -PARAMS::MAX_DELTA_TICKS, PARAMS::MAX_DELTA_TICKS); }

        /// @brief вращать мотор (вызывать постоянно)
        void spin() {
            setDirPWM(KP_SPEED * (next_pos - position));
            if (millis() < next_pos_timer) return;
            next_pos_timer = millis() + PARAMS::SPIN_PERIOD_MS;
            next_pos += speed;
            return;
        }

        /// @brief Стремиться к целевой позиции
        bool follow() {
            spin();
            int32_t u = (target - position) / PARAMS::KP_1_FOLLOW;
            speed = constrain(u, -abs(speed_set), abs(speed_set));
            return abs(u) > 0;
        }
    };
}