#pragma once

#include <Arduino.h>
#include "core/config.hpp"


namespace mosh {
namespace hardware {

/// @brief Обработчик левого мотора
void __l_int();

/// @brief Обработчик правого мотора
void __r_int();


/// @brief класс DC-мотор-энкодера, управляемого H-мостом
class MotorEncoder {
    const uint8_t PIN_SPEED, PIN_DIR, PIN_ENC_B;
    const bool INVERTED;

    uint8_t speed_set = 0;  // Модуль заданой скорости
    int8_t speed = 0;  // регулируемая скорость
    uint32_t timer = 0;  // таймер смены позиции
    int32_t next_pos = 0;  // следущая целевая позиция

    public:

    volatile int32_t position = 0;  // текущее положение

    /**
     * @brief Мотор-Энкодер управляемый H-мостом
     * @param encoder_handler обработчик энкодера в прерывании
     * @param inverted глобальная инверсия мотора (направления и энкодер)
     * @param pin_speed пин скорости (ШИМ)
     * @param pin_dir пин скорости (Цифровой)
     * @param pin_enc_a пин основного канала энкодера (Цифровой ПРЕРЫВАНИЕ)
     * @param pin_enc_b пин второго канала энкодера (Цифровой)
     */
    MotorEncoder(void (*encoder_handler) (void), bool inverted, uint8_t pin_speed, uint8_t pin_dir, uint8_t pin_enc_a, uint8_t pin_enc_b);

    /// @brief вызывается исключительно в прерывании
    void enc();

    /// @brief сброс
    void reset();

    /// @brief Установить направление
    /// @param backward (true - нормально, false - реверс)
    void setDir(bool backward);

    /// @brief Установить шим
    /// @param pwm ШИМ 0..255
    void setPWM(uint8_t pwm);

    /// @brief Установить шим-направление. Отрицательные значения соотвествуют реверсу. ШИМ берётся по модулю и ограничивается.
    /// @param power (-255..255)
    void setDirPWM(int16_t power);

    /// @brief Установить целевую скорость вращения
    /// @param dtick
    void setSpeed(int16_t dtick);

    /// @brief вращать мотор (вызывать постоянно)
    void spin();
};
}
} // namespace mosh