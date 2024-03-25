#pragma once

#include "core/enviroment.hpp"

namespace mosh {
namespace core {
// обработчики выхода
namespace quit {
/// @brief Интерфейс обработки завершения движения
class Quiter {
    /**
     * @brief Обработка события
     * @return true если ещё НЕ вышел, false когда следует прервать работу
     */
    public: virtual bool tick() const = 0;
};

/**
 * @brief Обработка выхода по таймеру
 */
class OnTimer : public Quiter {
    const uint32_t END_TIME;

    public:
    /**
     * @brief Выход по таймеру
     * @param duration время
     */
    OnTimer(uint16_t duration);
    bool tick() const override;
};

/**
 * @brief Обработка расстояния
 */
class IfDistanceSensorRead : public Quiter {
    hardware::DistanceSensor& sensor;
    const uint8_t DISTANCE;
    bool mode;

    public:
    enum MODE { LESS = 0, GREATER = 1 };

    /**
     * @brief Выход по расстоянию с датчика
     * @param used_sensor используемый датчик
     * @param target_distance целевое значение расстояния (см)
     * @param condition условие поддержания работы
     */
    IfDistanceSensorRead(hardware::DistanceSensor& used_sensor, const uint8_t target_distance, enum MODE condition);

    bool tick() const override;
};

} // namespace quit
}
}