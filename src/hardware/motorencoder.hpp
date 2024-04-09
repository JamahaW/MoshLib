#pragma once

#include <Arduino.h>

#define KP_SPEED 8
#define MAX_DELTA_TICKS 32
#define SPIN_PERIOD_MS 32

/// @brief Левый мотор скорость
#define MOTOR_L_DIR 4

/// @brief Левый мотор направление
#define MOTOR_L_SPEED 5

/// @brief Правый мотор скорость
#define MOTOR_R_SPEED 6

/// @brief Правый мотор направление
#define MOTOR_R_DIR 7 

#ifndef MOTOR_L_ENC_A
#define MOTOR_L_ENC_A 2 // Левый мотор энкодер желтый
#endif

#ifndef MOTOR_L_ENC_B
#define MOTOR_L_ENC_B 8 // Левый мотор энкодер зелёный
#endif

#ifndef MOTOR_L_INVERT
#define MOTOR_L_INVERT false // Левый мотор инверсия направления
#endif

#ifndef MOTOR_R_INVERT
#define MOTOR_R_INVERT true // Правый мотор инверсия направления
#endif

#ifndef MOTOR_R_ENC_A
#define MOTOR_R_ENC_A 3 // Правый мотор энкодер желтый
#endif

#ifndef MOTOR_R_ENC_B
#define MOTOR_R_ENC_B 9 // Правый мотор энкодер зелёный
#endif


namespace mosh {
namespace hardware {

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

/// @brief Левый мотор
extern mosh::hardware::MotorEncoder motorL;

/// @brief Правый мотор
extern mosh::hardware::MotorEncoder motorR;


namespace mosh {
namespace hardware {

/// @brief контроллер робота
struct MotorController {

    /// @brief Сброс данных моторов
    void reset();

    /// @brief Обновить поворот моторов по энкодеру
    void motorsSpin();

    /// @brief Задать целевые значения скоростей моторов
    /// @param left скорость-тик левого мотора
    /// @param right скорость-тик правого мотора
    void setSpeed(int16_t left, int16_t right);

    /// @brief Установить Направление-ШИМ для моторов
    /// @param dirpwm_left направление-ШИМ левого мотора
    /// @param dirpwm_right направление-ШИМ правого мотора
    void setDirPWM(int16_t dirpwm_left, int16_t dirpwm_right);
};

} // namespace hardware
} // namespace mosh

extern mosh::hardware::MotorController motors;