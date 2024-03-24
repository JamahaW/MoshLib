#include "MoshLib.hpp"


// КОМПОНЕНТЫ

hardware::LineSensor lineL(pin::L_SENSOR, PARAMS::L_LINE, PARAMS::L_FIELD);
hardware::LineSensor lineR(pin::R_SENSOR, PARAMS::R_LINE, PARAMS::R_FIELD);

hardware::NoDistanceSensor no_sensor;
hardware::IrSensorSharp ir0(pin::IR_0);
hardware::IrSensorSharp ir1(pin::IR_1);
hardware::UltraSonic us(pin::PIN_ECHO, pin::PIN_TRIG);

hardware::MotorEncoder motorL(hardware::__l_int, pin::ML_INVERT, pin::ML_SPEED, pin::ML_DIR, pin::ML_ENC_A, pin::ML_ENC_B);
hardware::MotorEncoder motorR(hardware::__r_int, pin::MR_INVERT, pin::MR_SPEED, pin::MR_DIR, pin::MR_ENC_A, pin::MR_ENC_B);


void hardware::__l_int() { motorL.enc(); }
void hardware::__r_int() { motorR.enc(); }

/**
 * @brief Данные о роботе
 */
static struct RobotConfig {
    hardware::DistanceSensor* dist_left = &no_sensor; // используется при движении вдоль стены слева
    hardware::DistanceSensor* dist_right = &no_sensor; // используештся при движении вдоль стены справа
    hardware::DistanceSensor* dist_front = &no_sensor; // используется при движении до объекта спереди
} robot;


void distSensorL(hardware::DistanceSensor& sensor) { robot.dist_left = &sensor; }

void distSensorR(hardware::DistanceSensor& sensor) { robot.dist_right = &sensor; }

void distSensorF(hardware::DistanceSensor& sensor) { robot.dist_front = &sensor; }

// управление моторами [[[  TODO ликвидировать!  ]]]
namespace motors {
void reset() {
    motorL.reset();
    motorR.reset();
}

void spin() {
    motorL.spin();
    motorR.spin();
}

void setSpeeds(int8_t left, int8_t right) {
    motorL.setSpeed(left);
    motorR.setSpeed(right);
}

void setForTicks(int8_t speed_L, int32_t ticks_L, int8_t speed_R, int32_t ticks_R) {
    reset();
    setSpeeds(speed_L, speed_R);
    motorL.target = ticks_L;
    motorR.target = ticks_R;

    bool runL = true;
    bool runR = true;

    while (runL || runR) {
        runL = motorL.follow();
        runR = motorR.follow();
    }

    goHold();
}
} // namespace motors

// Реализация алгоритмов и общих принципов работы
namespace moshcore {

// Регуляторы движения
namespace move {

/**
 * @brief Абстрактный исполнитель движения робота
 */
class Mover {

    protected:
    /**
     * @brief При необходимости может быть переопределён в наследниках
     */
    virtual void tick() const {};

    public:

    /**
     * @brief
     */
    void update() const {
        tick();
        motors::spin();
    }
};

class KeepSpeed : public Mover {
    /**
     * @brief Движение с поддержинием скорости TODO управление ускорением
     * @param speed целевая скорость
     */
    public: KeepSpeed(int8_t speed_left, int8_t speed_right) { motors::setSpeeds(speed_left, speed_right); }
};

class ProportionalRegulator : public Mover {
    private:

    const float KP = 0.3;
    uint8_t BASE_SPEED = 0;

    protected: void tick() const override {
        int8_t u = (lineL() - lineR()) * KP;
        motors::setSpeeds(BASE_SPEED - u, BASE_SPEED + u);
    }

    public: ProportionalRegulator(uint8_t speed) : BASE_SPEED(speed * 0.7) {}
};

} // namespace move

// движение по линии
namespace line {

/// @brief Абстрактный регулятор движения по линии
class Regulator {
    /// @brief выполнить расчёт системы
    protected: virtual void calc() const = 0;

    public:

    /**
     * @brief Инициализировать регулятор
     * @param speed базовая скорость движения
     */
    virtual void init(uint8_t speed) = 0;

    /// @brief Обновление системы. Вызывать как можно чаще в цикле движения
    void update() const {
        calc();
        motors::spin();
    }
};

/// @brief Абстрактный релейный регулятор
class Relay : public Regulator {
    protected: uint8_t SPEED_SET = 0, SECOND_SPEED = 0;
    public: void init(uint8_t speed) override {
        SPEED_SET = speed;
        SECOND_SPEED = -0.2 * speed;
    }
};

/// @brief Релейный регулятор по левому датчику
static class RelayL : public Relay {
    protected: void calc() const override {
        bool L = lineL.on();
        motors::setSpeeds(L ? SECOND_SPEED : SPEED_SET, L ? SPEED_SET : SECOND_SPEED);
    }
} relay_l;

/// @brief Релейный регулятор по правому датчику
static class RelayR : public Relay {
    protected: void calc() const override {
        bool R = lineR.on();
        motors::setSpeeds(R ? SPEED_SET : SECOND_SPEED, R ? SECOND_SPEED : SPEED_SET);
    }
} relay_r;

/// @brief Релейный регулятор по двум датчикам
static class RelayLR : public Relay {
    protected: void calc() const override {
        bool L = lineL.on(), R = lineR.on();
        motors::setSpeeds((L > R) ? SECOND_SPEED : SPEED_SET, (L < R) ? SECOND_SPEED : SPEED_SET);
    }
} relay_lr;

/// @brief Пропорциональный регулятор линии
static class Proportional : public Regulator {

    private:
    uint8_t BASE_SPEED = 0;
    float KP = 0;

    protected: void calc() const override {
        int8_t u = (lineL() - lineR()) * KP;
        motors::setSpeeds(BASE_SPEED - u, BASE_SPEED + u);
    }

    public: void init(uint8_t speed) override {
        BASE_SPEED = speed * 0.7;
        KP = 0.3;
    }
} proportional;

}

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
    private: const uint32_t END_TIME;

    public:

    /**
     * @brief Выход по таймеру
     * @param duration время
     */
    OnTimer(uint16_t duration) : END_TIME(millis() + duration) {}

    bool tick() const override { return millis() < END_TIME; }
};

/**
 * @brief Обработка расстояния
 */
class IfDistance : public Quiter {
    private:

    hardware::DistanceSensor& sensor;
    const uint8_t DISTANCE;
    bool mode;

    public:

    enum MODE {
        LESS = 0,
        GREATER = 1,
    };

    /**
     * @brief Выход по расстоянию с датчика
     * @param used_sensor используемый датчик
     * @param target_distance целевое значение расстояния (см)
     * @param condition условие поддержания работы
     */
    IfDistance(hardware::DistanceSensor& used_sensor, const uint8_t target_distance, enum MODE condition) :
        sensor(used_sensor), DISTANCE(target_distance), mode((bool) condition) {}

    bool tick() const override { return (sensor.read() <= DISTANCE) ^ mode; }
};

} // namespace quit

static void process(const move::Mover&& mover, const quit::Quiter&& quiter, bool hold_at_end = true) {
    while (quiter.tick()) mover.update();
    if (hold_at_end) goHold();
    motorL.setPWM(0);
    motorR.setPWM(0);
}

} // namespace moshcore

void goTime(uint32_t runtime, int8_t speed_left, int8_t speed_right, bool __hold_at_end) {
    using namespace moshcore;
    process(move::KeepSpeed(speed_left, speed_right), quit::OnTimer(runtime), __hold_at_end);
}

void goTime(uint32_t runtime, int8_t speed) { goTime(runtime, speed, speed); }

void goHold(uint32_t timeout) { goTime(timeout, 0, 0, false); }

void goTick(int32_t ticks, uint8_t speed) {
    motors::setForTicks(speed, ticks, speed, ticks);
}

// Переводной макрос из ММ пути в тики энкодера
#define MM2TICKS(mm) ( (int32_t)(mm) * 1000L / (int32_t)(PARAMS::MM_IN_1000_TICKS) ) // .. где мой constexpr, Ардуино!!? где С++11???

void goDist(int32_t distance_mm, uint8_t speed) {
    int32_t ticks = MM2TICKS(distance_mm);
    motors::setForTicks(speed, ticks, speed, ticks);
}

static void __go_proc_wall(hardware::DistanceSensor& sensor, uint8_t distance, int8_t speed, moshcore::quit::IfDistance::MODE mode) {
    using namespace moshcore;
    process(move::KeepSpeed(speed, speed), quit::IfDistance(sensor, distance, mode));
}

void goWallFront(hardware::DistanceSensor& sensor, uint8_t wall_dist_cm, uint8_t speed) {
    __go_proc_wall(sensor, wall_dist_cm, speed, moshcore::quit::IfDistance::MODE::GREATER);
}

void goWallFront(uint8_t distance, uint8_t speed) { goWallFront(*robot.dist_front, distance, speed); }

void goWallBack(hardware::DistanceSensor& sensor, uint8_t wall_dist_cm, uint8_t speed) {
    __go_proc_wall(sensor, wall_dist_cm, -speed, moshcore::quit::IfDistance::MODE::LESS);
}

void goWallBack(uint8_t distance, uint8_t speed) { goWallBack(*robot.dist_front, distance, speed); }

void turnAngle(int16_t a, uint8_t speed) {
    int32_t ticks = MM2TICKS((int32_t) a * PARAMS::TRACK * M_PI / 360.0);
    motors::setForTicks(speed, ticks, speed, -ticks);
}

void goLineTime(uint32_t runtime, uint8_t speed) {
    using namespace moshcore;
    process(move::ProportionalRegulator(speed), quit::OnTimer(runtime));
}


// ТЕСТИРОВАНИЕ

void test::lines(uint16_t& ret_L, uint16_t& ret_R, uint32_t timeout) {
    uint16_t i;
    uint32_t sum_L = 0, sum_R = 0;
    timeout += millis();

    for (i = 0; millis() < timeout; i++) {
        sum_L += lineL.readRaw();
        sum_R += lineR.readRaw();
    }

    ret_L = sum_L / i;
    ret_R = sum_R / i;
}


/**
 * @brief Изменение скорости регулятором
 * @param motor
 * @param dir
 */
static void test_motors_speed(hardware::MotorEncoder* motor, int8_t dir) {
    uint32_t timer = millis() + 120;

    motor->reset();

    for (int16_t speed = -PARAMS::MAX_DELTA_TICKS; speed <= PARAMS::MAX_DELTA_TICKS; speed++) {
        motor->setSpeed((PARAMS::MAX_DELTA_TICKS - abs(speed)) * dir);
        while (millis() < timer) motor->spin();
        timer += 120;
    }
    motor->setPWM(0);
    delay(500);
}

void test::motorsAccel() {
    hardware::MotorEncoder* _motors[]{ &motorL, &motorR };

    for (hardware::MotorEncoder* m : _motors) {
        test_motors_speed(m, 1);
        test_motors_speed(m, -1);
    }
}

void test::motorsPWM() {
    hardware::MotorEncoder* _motors[]{ &motorL, &motorR };

    for (hardware::MotorEncoder* motor : _motors) {
        for (int16_t i = -255; i < 256; i++) {
            motor->setPWM(255 - abs(i));
            motor->setDir(i > 0);
            delay(2 * 2000 / 256);
        }
        motor->setPWM(0);
        delay(500);
    }
}
