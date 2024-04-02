#pragma once

#include <Arduino.h>
#include "control/move.hpp"
#include "control/quit.hpp"


namespace mosh {
namespace run {

/**
 * @brief Запустить выполнение задачи
 * @param mover обработчик движения робота
 * @param quiter обработчик выхода
 * @param hold_at_end удержания моторов в конце
 */
void base(const mosh::control::Mover& mover, const mosh::control::Quiter& quiter, bool hold_at_end = true);

/**
 * @brief Выполнение задачи в течении времени
 * @param mover обработчик движения
 * @param runtime время работы (мс)
 */
void time(const mosh::control::Mover& mover, uint32_t runtime);

} // namespace run
}