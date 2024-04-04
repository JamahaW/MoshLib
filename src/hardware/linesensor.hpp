#pragma once

#include <Arduino.h>
#include "core/config.hpp"


namespace mosh {
namespace hardware {

/// @brief Аналоговый датчик линии
class LineSensor {

    /// @brief Аналоговый пин датчика
    uint8_t pin;

    /// @brief значение АЦП на линии
    uint16_t on_line = DEFAULT_CONF::LINE_ON_LINE;

    /// @brief значение АЦП на поле
    uint16_t on_field = DEFAULT_CONF::LINE_ON_FIELD;

    public:

    /// @brief Установить пин датчика
    /// @param pin Аналоговый Пин датчика (A0-A7)
    LineSensor& setPin(uint8_t pin);

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

    /**
     * @brief Проверить логическое наличие линии
     * @return (%) линии больше `GRAY_PERCENT`
     */
    bool on() const;
};

}
} // namespace mosh
