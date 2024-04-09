#pragma once

#include <Arduino.h>
#include "hardware/linesensor.hpp"
#include "hardware/distancesensor.hpp"
#include "util.hpp"


namespace mosh {
namespace control {

/// @brief Абстрактный исполнитель движения робота
class Mover {
    public:

    /// @brief Обновление системы
    virtual void tick() const;
};

/// @brief режим постоянной скорости
class KeepSpeed : public Mover {
    public:
    KeepSpeed(int8_t speed_left, int8_t speed_right);
    KeepSpeed(int8_t speed);
    void tick() const override;
};

/// @brief Синхронизированое движение моторов
class Sync : public Mover {
    const int16_t FACT_L; // Скорость левого
    const int16_t FACT_R; // Скорость правого
    const int8_t KL; // К левого
    const int8_t KR; // К правого
    const int8_t SIG_L;
    const int8_t SIG_R;

    public:

    /**
     * @brief Синхронное вращение моторов
     * @param fact_l Скорость левого мотора
     * @param fact_r Скорость правого мотора
     * @param kl Множитель смещения левого
     * @param kr Множитель смещения правого
     */
    Sync(int16_t fact_l, int16_t fact_r, int8_t kl, int8_t kr);
    /**
     * @brief Паралельный сихронный регулятор
     * @param fact скорость ШИМ
     */
    Sync(int16_t fact);

    void tick() const override;
};

/// @brief Пропорциональный регулятор движения по линии
class LineProp : public Mover {
    const float KP = 0.1; // TODO вынести коэф
    uint8_t BASE_SPEED;
    protected: void tick() const override;
    public: LineProp(int8_t speed);
};

class LineRelay : public Mover {
    const int8_t SPEED_A, SPEED_B;
    mosh::hardware::LineSensor* sensor;

    public:

    /**
     * @brief Релейный регулятор движения по линии по одному датчику
     * @param speed скорость перемещения
     * @param sensor_dir положение датчика
     */
    LineRelay(Directions sensor_dir, int8_t speed);
    void tick() const override;
};

/// @brief Релейный регулятор движения по линии по двум датчикам
class LineRelay2 : public Mover {
    int8_t SPEED, SECOND;

    public:
    LineRelay2(int8_t speed);
    void tick() const override;
};

class MoveAlongWall : public Mover {
    const int16_t SPEED;
    const uint8_t TARGET;
    const float k;
    mosh::hardware::DistanceSensor* sensor;

    public:

    /**
     * @brief Движение вдоль стены по датчку расстояния СЛЕВА или СПРАВА
     * @param distance целевое расстояние см
     * @param pos положение датчика `DIST_LEFT` | `DIST_RIGHT`
     */
    MoveAlongWall(uint8_t distance, Directions pos, int8_t speed);
    void tick() const override;
};



}
} // namespace mosh
