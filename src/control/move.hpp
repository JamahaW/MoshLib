#pragma once

#include <Arduino.h>
#include "core/enviroment.hpp"


namespace mosh {
namespace control {

/**
 * @brief Абстрактный исполнитель движения робота
 */
class Mover {
    protected:
    /// @brief При необходимости может быть переопределён в наследниках
    virtual void tick() const;

    public:

    /// @brief обновление системы
    void update() const;
};

/// @brief режим постоянной скорости
class KeepSpeed : public Mover {
    public: KeepSpeed(int8_t speed_left, int8_t speed_right);
};

/// @brief Пропорциональный регулятор движения по линии
class LineProportional : public Mover {
    const float KP = 0.3;
    uint8_t BASE_SPEED;
    protected: void tick() const override;
    public: LineProportional(int8_t speed);
};

class LineRelaySingle : public Mover {
    private:
    const int8_t SPEED_A, SPEED_B;
    hardware::LineSensor* sensor;

    public:

    enum SENSOR { LINE_LEFT, LINE_RIGHT };

    /**
     * @brief Релейный регулятор движения по линии по одному датчику
     * @param speed скорость перемещения
     * @param sensor_dir положение датчика `SENSOR::LEFT` | `SENSOR::RIGHT`
     */
    LineRelaySingle(SENSOR sensor_dir, int8_t speed);
    void tick() const override;
};

/// @brief Релейный регулятор движения по линии по двум датчикам
class LineRelayBoth : public Mover {
    int8_t SPEED, SECOND;

    public:
    LineRelayBoth(int8_t speed);
    void tick() const override;
};

class MoveAlongWall : public Mover {
    int16_t SPEED;
    const uint8_t TARGET;
    const float k;
    hardware::DistanceSensor* sensor;

    public:

    enum POS { DIST_LEFT = -1, DIST_RIGHT = 1 };

    /**
     * @brief Движение вдоль стены по датчку расстояния СЛЕВА или СПРАВА
     * @param distance целевое расстояние см
     * @param pos положение датчика `DIST_LEFT` | `DIST_RIGHT`
     */
    MoveAlongWall(uint8_t distance, POS pos, int8_t speed);
    void tick() const override;
};

}
} // namespace mosh
