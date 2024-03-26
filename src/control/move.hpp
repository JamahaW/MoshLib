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
    virtual ~Mover(); // для delete TODO искоренить new !!
};

class KeepSpeed : public Mover {
    /**
     * @brief Движение с поддержинием скорости TODO управление ускорением
     * @param speed целевая скорость
     */
    public: KeepSpeed(int8_t speed_left, int8_t speed_right);
};

class ProportionalLineRegulator : public Mover {
    const float KP = 0.3;
    uint8_t BASE_SPEED = 0;

    protected: void tick() const override;

    public:
    /**
     * @brief Пропорциональный регулятор движения по линии
     * @param speed скорость движения
     */
    ProportionalLineRegulator(uint8_t speed);
};

class RelayLineSingle : public Mover {
    int8_t SPEED_B, SPEED_A;
    hardware::LineSensor* sensor;

    public:
    enum SENSOR { LINE_LEFT, LINE_RIGHT };

    /**
     * @brief Релейный регулятор движения по линии по одному датчику
     * @param speed скорость перемещения
     * @param sensor_dir положение датчика `SENSOR::LEFT` | `SENSOR::RIGHT`
     */
    RelayLineSingle(uint8_t speed, enum SENSOR sensor_dir);

    void tick() const override;
};

class RelayLineBoth : public Mover {
    int8_t SPEED, SECOND;

    public:
    /**
     * @brief Релейный регулятор движения по линии по двум датчикам
     * @param speed
     */
    RelayLineBoth(uint8_t speed);

    void tick() const override;
};

class MoveAlongWall : public Mover {
    const int16_t SPEED;
    const uint8_t TARGET;
    const float k;
    hardware::DistanceSensor* sensor;

    public:

    enum POS { DIST_LEFT = -1, DIST_RIGHT = 1 };

    /**
     * @brief Движение вдоль стены по датчку расстояния СЛЕВА или СПРАВА
     * @param speed скорость движения
     * @param target_distance_cm целевое расстояние см
     * @param direction положение датчика `DIST_LEFT` | `DIST_RIGHT`
     */
    MoveAlongWall(int8_t speed, uint8_t target_distance_cm, POS direction);
    void tick() const override;
};

}
} // namespace mosh
