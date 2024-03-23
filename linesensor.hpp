// датчики линии

#pragma once

#include <Arduino.h>
#include "config.hpp"

/**
 * @brief Аналоговый датчик линии
 */
class LineSensor {

private:

    /// @brief пин: A0 - A7
    const uint8_t PIN;

    /// @brief значение на линии
    const uint16_t ON_LINE;

    /// @brief значение на поле
    const uint16_t ON_FIELD;

public:

    /**
     * @brief Аналоговый датчик линии
     * @param pin Аналоговый Пин датчика (A0-A7)
     * @param on_line Значение АЦП с пина датчика на линии
     * @param on_field Значение АЦП с пина датчика на поле
     */
    LineSensor(
        const uint8_t pin,
        const uint16_t on_line,
        const uint16_t on_field
    ) : PIN(pin), ON_LINE(on_line), ON_FIELD(on_field) {
        pinMode(PIN, INPUT);
    }

    /**
     * @brief Считать значение с АЦП
     * @return уровень сигнала 0 - 1024
     */
    int16_t readRaw() { return analogRead(PIN); }

    /**
     * @brief определить на сколько (%) датчик находится на линии
     * @return уровень нахождения датчика на линии (0 .. 100)
     */
    int8_t read() {
        int16_t ret = map(readRaw(), ON_LINE, ON_LINE, 100, 0);
        return (int8_t)constrain(ret, 0, 100);;
    }
}
lineLeft(cfg::LINE::L_SENSOR, cfg::LINE::L_LINE, cfg::LINE::L_FIELD),
lineRight(cfg::LINE::R_FIELD, cfg::LINE::R_LINE, cfg::LINE::R_FIELD);
