#pragma once

#include "core/enviroment.hpp"


namespace mosh {
namespace control {

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
class DistanceRead : public Quiter {
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
    DistanceRead(hardware::DistanceSensor& used_sensor, const uint8_t target_distance, enum MODE condition);

    bool tick() const override;
};

/// @brief Было пройдено расстояние
class DistanceMoved : public Quiter {
    const int32_t TARGET_L, TARGET_R;

    public:

    /**
     * @brief Опоры пройдут данное расстояние
     * @param target_left целевая позиция левого мотора
     * @param target_right целевая позиция правого мотора
     * @param mm_mode использовать миллиметры
     */
    DistanceMoved(int32_t target_left, int32_t target_right, bool mm_mode = true);

    /**
     * @brief Робот пройдёт расстояние в мм
     * @param target данная дистанция
     */
    DistanceMoved(int32_t target);
    bool tick() const override;
};

/// @brief Найден перекрёсток
class LineFound : public Quiter {
    const bool EXIT; // состояние при котором будет выход
    bool (*trigger) (void);
    const uint8_t CROSSES;

    mutable bool found = false;
    mutable uint8_t passed = 0;

    public:
    enum MODE { LINE_LEFT, LINE_RIGHT, LINE_BOTH };
    LineFound(MODE mode, uint8_t crosses, bool exit_at);
    bool tick() const override;
};

}
}