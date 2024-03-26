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
    /// @brief задать скорость движения
    virtual const Mover& init(int8_t speed_left, int8_t speed_right) { return *this; }

    /// @brief задать общую скорость двмжения
    virtual const Mover& init(int8_t speed) { return *this; }

    /// @brief обновление системы
    void update() const;
};

/// @brief режим постоянной скорости
class KeepSpeed : public Mover {
    public: const Mover& init(int8_t speed_left, int8_t speed_right) override;
};

/// @brief Пропорциональный регулятор движения по линии
class ProportionalLineRegulator : public Mover {
    const float KP = 0.3;
    uint8_t BASE_SPEED = 0;
    protected: void tick() const override;
    public: const Mover& init(int8_t speed) override;
};

class RelayLineSingle : public Mover {
    public: enum SENSOR { LINE_LEFT, LINE_RIGHT };

    private:
    SENSOR __dir; // TODO использовать сравнения указателей на сенсор

    int8_t SPEED_B, SPEED_A;
    hardware::LineSensor* sensor;

    public:

    /**
     * @brief Релейный регулятор движения по линии по одному датчику
     * @param speed скорость перемещения
     * @param sensor_dir положение датчика `SENSOR::LEFT` | `SENSOR::RIGHT`
     */
    RelayLineSingle(SENSOR sensor_dir);
    const Mover& init(int8_t speed) override;
    void tick() const override;


};

/// @brief Релейный регулятор движения по линии по двум датчикам
class RelayLineBoth : public Mover {
    int8_t SPEED, SECOND;

    public:
    const Mover& init(int8_t speed) override;
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
    MoveAlongWall(uint8_t distance, POS pos);
    void tick() const override;
    const Mover& init(int8_t speed) override;
};

}
} // namespace mosh
