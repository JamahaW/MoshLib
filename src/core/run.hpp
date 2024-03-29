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

/**
 * @brief Запустить независимое вращение моторов с заданой скоростью с проверкой
 * @param speed скорость моторов
 * @param quiter обработчик выхода
 */
void speed(int8_t speed, const mosh::control::Quiter& quiter);


// /**
//  * @brief Движение с обработчиком пока передний датчик не увидит стену на расстоянии
//  * @param mover обработчик
//  * @param stop_dist расстояния остановки
//  */
 // static void wall(const Mover& mover, DistanceSensor& sensor, uint8_t stop_dist) { full(mover, IfDistanceSensorRead(sensor, stop_dist, IfDistanceSensorRead::GREATER)); }

 // static void speed(int8_t left, int8_t right, const Quiter& quiter) { full(KeepSpeed(left, right), quiter); }
} // namespace run
}