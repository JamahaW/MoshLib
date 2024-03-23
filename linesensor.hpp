// датчики линии

#pragma once

#include <Arduino.h>


/// @brief Аналоговый Датчик линии
class LineSensor {
    const uint8_t PIN;
    const uint16_t on_line;
    const uint16_t on_field;
};
