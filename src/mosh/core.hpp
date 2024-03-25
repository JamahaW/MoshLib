#pragma once

#include <Arduino.h>

namespace mosh {

// ядро функциональности
namespace core {

// Регуляторы движения
namespace move {

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
    private:
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
    private:
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
    private: int8_t SPEED, SECOND;

    public:
    /**
     * @brief Релейный регулятор движения по линии по двум датчикам
     * @param speed
     */
    RelayLineBoth(uint8_t speed);

    void tick() const override;
};

class MoveAlongWall : public Mover {
    private:

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

Mover* getLineRegulator(LINE_REGULATORS type, uint8_t speed) {
    switch (type) {
        case LINE_REGULATORS::RELAY_L: return new RelayLineSingle(speed, RelayLineSingle::LINE_LEFT);
        case LINE_REGULATORS::RELAY_R: return new RelayLineSingle(speed, RelayLineSingle::LINE_RIGHT);
        case LINE_REGULATORS::RELAY_LR: return new RelayLineBoth(speed);
        case LINE_REGULATORS::PROP: return new ProportionalLineRegulator(speed);
        default: return new Mover;
    }
}

} // namespace move

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

static void run(const move::Mover& mover, const quit::Quiter& quiter, bool hold_at_end = true) {
    while (quiter.tick()) mover.update();
    if (hold_at_end) goHold();
    motorL.setPWM(0);
    motorR.setPWM(0);
}

} // namespace core
} // namespace mosh
