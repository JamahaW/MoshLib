#pragma once

#include <Arduino.h>
#include "core/values.hpp"


namespace mosh {
namespace hardware {

/// @brief Интерфейс датчика растояния
class DistanceSensor {

    /// @brief нижняя граница расстояния
    const uint16_t DIST_FLOOR;

    /// @brief верхняя граница расстояния
    const uint16_t DIST_CEIL;

    /// @brief период изменения значения
    const uint8_t PERIOD;

    /// @brief таймер для периодического опрашивания
    mutable uint32_t timer = 0;

    /// @brief предыдущее значение датчика
    mutable uint8_t value = 0;

    protected:

    /// @brief Опросить датчик
    /// @return значение расстояние в мм
    virtual uint8_t update() const = 0;

    public:

    /**
     * @brief Абстрактный Датчик расстояния
     * @param min_dist Нижняя граница отсечения расстояния
     * @param max_dist Верхняя граница отсеченя расстояния
     * @param period Период чтения (мс)
     */
    DistanceSensor(uint16_t min_dist, uint16_t max_dist, uint8_t period);

    /**
     * @brief Получить значение с датчика
     * @return расстояние датчика (см)
     */
    uint8_t read() const;


    /// @brief Оператор Функтора для упрощения
    /// @return как и метод read()
    uint8_t operator () () const;
};


/// @brief Заглушка, возвращает всегда 0
class NoDistanceSensor : public DistanceSensor {
    protected: uint8_t update() const override { return 0; }
    public: NoDistanceSensor() : DistanceSensor(0, 0, 0) {}
};


/// @brief Инфокрасный датчик Sharp
class IrSensorSharp : public DistanceSensor {

    /// @brief Аналоговый пин датчика
    uint8_t pin;

    protected: uint8_t update() const override;

    public:

    /// @brief Инфокрасный датчик Sharp
    IrSensorSharp();

    /// @brief Задать пин датчика
    /// @param pin Аналоговый пин (A0 - A7)
    void pinout(uint8_t pin);
};

/// @brief УльтраЗвуковой датчик HC SR04
class UltraSonic : public DistanceSensor {

    /// @brief Пин приёма
    uint8_t ECHO;

    /// @brief Пин триггера
    uint8_t TRIG;

    protected: uint8_t update() const override;

    public:

    /// @brief Ультразвуковой датчик расстояния
    UltraSonic();

    /// @brief задать распиновку
    /// @param pin_echo Пин приёма
    /// @param pin_trig Пин Триггера
    void pinout(uint8_t pin_echo, uint8_t pin_trig);
};

}
} // namespace mosh


/// @brief Заглушка
extern mosh::hardware::NoDistanceSensor __no_sensor;

/// @brief Левый ИК-датчик расстояния
extern mosh::hardware::IrSensorSharp ir0;

/// @brief Правый ИК-датчик расстояния
extern mosh::hardware::IrSensorSharp ir1;

/// @brief Ультразвуковой датчик расстояния
extern mosh::hardware::UltraSonic us;
