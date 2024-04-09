#pragma once

/// @brief Направления (для датчиков и т.п.)
enum Directions { LEFT = 1, RIGHT = -1 };

extern unsigned int __bss_end;
extern void* __brkval;

/// @brief Получить занимаемую память
/// @return ОЗУ свободно (байт)
uint16_t getFreeSram() {
  uint8_t v;
  return ((uint16_t)__brkval == 0) ? (((uint16_t)&v) - ((uint16_t)&__bss_end)) : (((uint16_t)&v) - ((uint16_t)__brkval));
};