#pragma once

#include <Arduino.h>


namespace mosh {
namespace hardware {

/// @brief Аналоговый датчик линии
class LineSensor {

    /// @brief Аналоговый пин датчика
    uint8_t pin;

    /// @brief значение АЦП на линии
    uint16_t on_line = 82;

    /// @brief значение АЦП на поле
    uint16_t on_field = 909;

    public:

    /// @brief Установить пин датчика
    /// @param pin Аналоговый Пин датчика (A0-A7)
    LineSensor& pinout(uint8_t pin);

    /// @brief
    /// @param on_line Значение АЦП с пина датчика на линии
    /// @param on_field Значение АЦП с пина датчика на поле
    LineSensor& config(uint16_t on_line, uint16_t on_field);

    /**
     * @brief Считать значение с АЦП
     * @return уровень сигнала 0 - 1024
     */
    int16_t readRaw() const;

    /**
     * @brief определить на сколько (%) датчик находится на линии
     * @return уровень нахождения датчика на линии (0 .. 100)
     */
    int8_t read() const;

    /**
     * @brief Оператор Функтора для сокращения
     * @return Процент нахождения датчика на линии
     */
    int8_t operator () () const;

    /// @brief Проверить логическое наличие линии
    /// @return значение с датчика выше порога
    bool on() const;
};

}
} // namespace mosh

/// @brief Левый датчик линии
extern mosh::hardware::LineSensor lineL;

/// @brief Правый датчик линии
extern mosh::hardware::LineSensor lineR;