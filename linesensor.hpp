// датчики линии

#pragma once

#include <Arduino.h>


/// @brief Аналоговый Датчик линии
class LineSensor {

private:

    const uint8_t PIN;
    const uint16_t ON_LINE;
    const uint16_t ON_FIELD;

public:

    LineSensor(
        const uint8_t pin,
        const uint16_t on_line,
        const uint16_t on_field
    ) : PIN(pin), ON_LINE(on_line), ON_FIELD(on_field) {
        pinMode(PIN, INPUT);
    }
};
