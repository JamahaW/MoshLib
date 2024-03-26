#pragma once

#include <Arduino.h>
#include "..\core\config.hpp"

namespace mosh {
namespace hardware {

/// @brief Интерфейс датчика растояния
class DistanceSensor {
    const uint16_t MIN_DISTANCE, MAX_DISTANCE;
    const uint8_t READ_PERIOD;
    mutable uint32_t next_read = 0; // время следующего опроса датчика
    mutable uint8_t value = 0; // предыдущее значение датчика

    protected:

    /**
     * @brief Опросить датчик
     * @return значение расстояние в мм
     */
    virtual uint8_t update() const = 0;

    public:

    /**
     * @brief Абстрактный Датчик расстояния
     * @param min_dist Нижняя граница отсечения расстояния
     * @param max_dist Верхняя граница отсеченя расстояния
     * @param period Период чтения (мс)
     */
    DistanceSensor(const uint16_t min_dist, const uint16_t max_dist, const uint8_t period);

    /**
     * @brief Получить значение с датчика
     * @return расстояние датчика (см)
     */
    uint8_t read() const;
    uint8_t operator () () const;
};


/// @brief Зашлушка
class NoDistanceSensor : public DistanceSensor {
    protected: uint8_t update() const override { return 0; }
    public: NoDistanceSensor() : DistanceSensor(0, 0, 0) {}
};


/// @brief Инфокрасный датчик Sharp
class IrSensorSharp : public DistanceSensor {
    const uint8_t PIN;

    protected:
    uint8_t update() const override;

    public:
    /// @brief ИК-датчик расстояния
    /// @param pin Аналоговый пин (A0 - A7)
    IrSensorSharp(const uint8_t pin);
};

/// @brief УльтраЗвуковой датчик HC SR04
class UltraSonic : public DistanceSensor {
    const uint8_t PIN_ECHO, PIN_TRIG;

    protected:

    uint8_t update() const override;

    public:

    /**
     * @brief Ультразвуковой датчик расстояния
     * @param pin_echo Пин приёма
     * @param pin_trig Пин Триггера
     */
    UltraSonic(const uint8_t pin_echo, const uint8_t pin_trig);

};

}
} // namespace mosh