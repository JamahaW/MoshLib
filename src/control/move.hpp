#pragma once

#include <Arduino.h>
#include "core/enviroment.hpp"


namespace mosh {
namespace control {

/**
 * @brief Абстрактный исполнитель движения робота
 */
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
    const int16_t SPEED;
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

/**
 * @brief Синхронизированое движение моторов
 */
class MoveSync : public Mover {
    const int16_t FACT_L; /// Смещение левого
    const int16_t FACT_R; /// Смещение правого
    const int8_t KL; /// К левого
    const int8_t KR; /// К правого
    const int8_t KE; /// Множитель ошибки энкодеров
    const int8_t SIG_L;
    const int8_t SIG_R;
    
    public:
    
    /**
     * @brief Синхронное вращение моторов
     * @param fact_l Смещение левого мотора
     * @param fact_r Смещение правого мотора
     * @param kl Множитель смещения левого
     * @param kr Множитель смещения правого
     * @param ke Множитель ошибки
     */
    MoveSync(int16_t fact_l, int16_t fact_r, int8_t kl, int8_t kr, int8_t ke);
    MoveSync(int16_t fact, int8_t kf, int8_t ke);
    
    void tick() const override;
};

}
} // namespace mosh
