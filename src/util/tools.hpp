#pragma once

#include <Arduino.h>

/// @brief Направления (для датчиков и т.п.)
enum Directions { LEFT = 1, RIGHT = -1 };

/// @brief Получить занимаемую память
/// @return ОЗУ свободно (байт)
uint16_t getFreeSram();