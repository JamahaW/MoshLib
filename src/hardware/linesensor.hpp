#pragma once

#include <Arduino.h>
#include "config.hpp"


namespace hardware {

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
        int16_t readRaw() const { return analogRead(PIN); }

        /**
         * @brief определить на сколько (%) датчик находится на линии
         * @return уровень нахождения датчика на линии (0 .. 100)
         */
        int8_t read() const {
            int16_t ret = map(readRaw(), ON_LINE, ON_FIELD, 100, 0);
            return (int8_t)constrain(ret, 0, 100);
        }

        int8_t operator () () const { return read(); }

        /**
         * @brief Проверить логическое наличие линии
         * @return (%) линии больше `GRAY_PERCENT`
         */
        bool on() { return read() > PARAMS::GRAY_PERCENT; }
    };

} // namespace sys