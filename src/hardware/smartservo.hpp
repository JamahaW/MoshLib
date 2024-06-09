#pragma once

#include <Arduino.h>
#include <Servo.h>


namespace mosh {
namespace hardware {

#define SMARTSERVO_TURN_TIMEOUT (100)
#define SMARTSERVO_DEFAULT_DT (5)
#define SMARTSERVO_DEFAULT_ANGLE (0)

class SmartServo {

    mutable Servo servo;
    uint8_t PIN;
    bool AUTODETACH;
    uint8_t dt;

    mutable uint8_t last_angle = 0;

    public:

    /// @brief Включить серво (Servo::attach(PIN))
    void use() const;

    /// @brief Выключить серво (Servo::detach(PIN))
    void disable() const;
    
    /// @brief установить угол поворота
    /// @param angle угол
    void set(uint8_t angle) const;

    /// @brief Блокирующая функция, Плавно установить угол поворота
    /// @param angle угол
    void turn(uint8_t angle) const;

    /// @brief Задать пин сервопривода
    /// @param pin пин сервопривода
    /// @return Ссылка на себя
    SmartServo& pinout(uint8_t pin);

    /// @brief Конфигурация сервопривода
    /// @param autodetach настроить автоматическое отключение сервопривода после поворота
    /// @param dt дельта времени поворота на градус (используется в SmartServo::turn)
    /// @param init_angle Изначальный угол поворота сервопривода.
    /// @return 
    SmartServo& config(bool autodetach, uint8_t dt = SMARTSERVO_DEFAULT_DT, uint8_t init_angle = SMARTSERVO_DEFAULT_ANGLE);
};

} // namespace hardware
} // namespace mosh

/// @brief Сервопривод 1
extern mosh::hardware::SmartServo servo1;
