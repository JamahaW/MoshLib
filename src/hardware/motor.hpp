#pragma once

#include <Arduino.h>

#define MOTOR_ENCODER_SPEED_KP 8
#define MAX_DELTA_TICKS 32
#define SPIN_PERIOD_MS 32


namespace mosh {
namespace hardware {

// мотор, управляемый драйвером L298N
class MotorL298N {

    /// @brief пин скорости
    uint8_t SPEED;

    /// @brief пин направления 
    uint8_t DIR;

    /// @brief инвертировать поведение
    bool INVERT;

    public:

    /// @brief Конфигурировать пины мотора
    /// @param speed ШИМ пин скорости
    /// @param dir цифровой пин направления
    /// @param invert инверсия направления
    /// @return ссылка на себя
    MotorL298N& pinout(uint8_t speed, uint8_t dir, bool invert);

    /// @brief Задать направление
    /// @param reserse инверсия движения
    void setDir(bool reserse);

    /// @brief Задать ШИМ
    /// @param pwm шим 0 .. 255
    void setPWM(uint8_t pwm);

    /// @brief Задать шим и направление
    /// @param dir_pwm значение -255 .. 255
    void setDirPWM(int16_t dir_pwm);
};

/// @brief класс DC-мотор-энкодера, управляемого H-мостом
class MotorEncoder : public MotorL298N {

    /// @brief Обработчик прерывания
    void (*handler) (void);

    /// @brief Пин второго канала энкодера
    uint8_t ENC_B;

    /// @brief Сменить направление энкодера
    bool ENC_INV;

    /// @brief регулируемая скорость
    int8_t d_pos = 0;

    /// @brief таймер смены позиции
    uint32_t timer = 0;

    /// @brief следущая целевая позиция
    int32_t next_pos = 0;

    public:

    volatile int32_t position = 0;  // текущее положение

    /// @brief Мотор-энкодер
    /// @param encoder_handler обработчик прерывания 
    MotorEncoder(void (*encoder_handler) (void));

    /// @brief Распиновка энкодера
    /// @param enc_a главный канал (ПРЕРЫВАНИЕ)
    /// @param enc_b второй канал
    /// @param invert инвертировать поведения энкодера
    /// @return ссылка на себя
    MotorEncoder& encoder(uint8_t enc_a, uint8_t enc_b, bool invert);

    /// @brief вызывается исключительно в прерывании
    void enc();

    /// @brief сброс
    void reset();

    /// @brief Установить скорость вращения
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
    void spin();

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