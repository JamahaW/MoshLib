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
    LINE_REGULATORS line_follow_regulator = LINE_REGULATORS::PROP; // Регулятор движения по линии по умолчанию
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

    virtual ~Mover() {} // для delete TODO искоренить new !!
};

class KeepSpeed : public Mover {
    /**
     * @brief Движение с поддержинием скорости TODO управление ускорением
     * @param speed целевая скорость
     */
    public: KeepSpeed(int8_t speed_left, int8_t speed_right) { motors::setSpeeds(speed_left, speed_right); }
};

class ProportionalLineRegulator : public Mover {
    private:

    const float KP = 0.3;
    uint8_t BASE_SPEED = 0;

    protected: void tick() const override {
        int8_t u = (lineL() - lineR()) * KP;
        motors::setSpeeds(BASE_SPEED - u, BASE_SPEED + u);
    }

    public:

    /**
     * @brief Пропорциональный регулятор движения по линии
     * @param speed скорость движения
     */
    ProportionalLineRegulator(uint8_t speed) : BASE_SPEED(speed * 0.7) {}
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
    RelayLineSingle(uint8_t speed, enum SENSOR sensor_dir) {
        SPEED_A = SPEED_B = (int8_t) speed;
        const int8_t second = -speed * 0.3;

        switch (sensor_dir) {

            case SENSOR::LINE_LEFT:
                sensor = &lineL;
                SPEED_A = second;
                break;

            case SENSOR::LINE_RIGHT:
                sensor = &lineR;
                SPEED_B = second;
                break;

        }
    }

    void tick() const override {
        bool on = sensor->on();
        motors::setSpeeds(on ? SPEED_A : SPEED_B, on ? SPEED_B : SPEED_A);
    }

};

class RelayLineBoth : public Mover {

    private: int8_t SPEED, SECOND;

    public:

    /**
     * @brief Релейный регулятор движения по линии по двум датчикам
     * @param speed
     */
    RelayLineBoth(uint8_t speed) : SPEED(speed), SECOND((int8_t) -speed * 0.3) {}

    void tick() const override {
        bool L = lineL.on(), R = lineR.on();
        motors::setSpeeds((L > R) ? SECOND : SPEED, (L < R) ? SECOND : SPEED);
    }
};

class MoveAlongWall : public Mover {

    private:

    const int16_t SPEED;
    const uint8_t TARGET;
    hardware::DistanceSensor* sensor;

    public:

    MoveAlongWall(int8_t speed, uint8_t target_distance_cm, hardware::DistanceSensor* sens) :
        SPEED(speed), TARGET(target_distance_cm), sensor(sens) {}

    void tick() const override {
        int16_t u = float(TARGET - sensor->read()) * -0.4;
        u = constrain(u, -SPEED, SPEED);
        motors::setSpeeds(SPEED - u, SPEED + u);
    }
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

    enum MODE { LESS = 0, GREATER = 1 };

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

static void process(const move::Mover& mover, const quit::Quiter&& quiter, bool hold_at_end = true) {
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

void lineReg(enum LINE_REGULATORS default_regulator) { robot.line_follow_regulator = default_regulator; }

void goLineTime(enum LINE_REGULATORS type, uint32_t runtime, uint8_t speed) {
    using namespace moshcore;
    move::Mover* mover = move::getLineRegulator(type, speed);
    process(*mover, quit::OnTimer(runtime));
    delete mover;
}

void goLineTime(uint32_t runtime, uint8_t speed) { goLineTime(robot.line_follow_regulator, runtime, speed); }

void goLwallTime(uint8_t distance, uint32_t runtime, uint8_t speed) {
    using namespace moshcore;
    process(move::MoveAlongWall(speed, distance, robot.dist_left), quit::OnTimer(runtime));
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
