#pragma once

#include <Arduino.h>
#include "core/config.hpp"


namespace mosh {
namespace hardware {

/**
 * @brief Аналоговый датчик линии
 */
class LineSensor {
    const uint8_t PIN;
    const uint16_t ON_LINE, ON_FIELD;

    public:

    /**
     * @brief Аналоговый датчик линии
     * @param pin Аналоговый Пин датчика (A0-A7)
     * @param on_line Значение АЦП с пина датчика на линии
     * @param on_field Значение АЦП с пина датчика на поле
     */
    LineSensor(uint8_t pin, uint16_t on_line, uint16_t on_field);

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
    bool on();
};

}
} // namespace mosh
