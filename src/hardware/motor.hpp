#pragma once

#include <Arduino.h>
#include "config.hpp"


namespace hardware
{
    /// @brief Обработчик левого мотора
    void __l_int();

    /// @brief Обработчик правого мотора
    void __r_int();


    /// @brief класс DC-мотор-энкодера, управляемого H-мостом
    class Motor {

    private:
        const uint8_t PIN_SPEED, PIN_DIR, PIN_ENC_B;
        const bool INVERTED;

        /// @brief Модуль заданой скорости
        uint8_t speed_set = 0;

        /// @brief регулируемая скорость
        int8_t speed = 0;

        /// @brief таймер смены позиции
        uint32_t next_pos_timer = 0;

        /// @brief следущая целевая позиция
        int32_t next_pos = 0;

        /// @brief текущее положение
        volatile int32_t position = 0;

    public:
        /// @brief цель движения
        int32_t target = 0;

        /**
         * @brief Мотор-Энкодер управляемый H-мостом
         * @param encoder_handler обработчик энкодера в прерывании
         * @param inverted глобальная инверсия мотора (направления и энкодер)
         * @param pin_speed пин скорости (ШИМ)
         * @param pin_dir пин скорости (Цифровой)
         * @param pin_enc_a пин основного канала энкодера (Цифровой ПРЕРЫВАНИЕ)
         * @param pin_enc_b пин второго канала энкодера (Цифровой)
         */
        Motor(void (*encoder_handler) (void), const bool inverted, const uint8_t pin_speed, const uint8_t pin_dir, const uint8_t pin_enc_a, const uint8_t pin_enc_b);

        /// @brief вызывается исключительно в прерывании
        void enc();

        /// @brief сброс данных мотора
        void reset();

        /// @brief Установить направление (true - нормально, false - реверс)
        /// @param backward 
        void setDir(bool backward);

        /// @brief Установить шим
        /// @param pwm ШИМ 0..255
        void setPWM(uint8_t pwm);

        /// @brief Установить шим-направление. Отрицательные значения соотвествуют реверсу. ШИМ берётся по модулю и ограничивается.
        /// @param power (-255..255)
        void setDirPWM(int16_t power);

        /// @brief Установить целевую скорость вращения
        /// @param dtick
        void setSpeed(int8_t dtick);

        /// @brief вращать мотор (вызывать постоянно)
        void spin();

        /// @brief Стремиться к целевой позиции
        bool follow();
    };
}