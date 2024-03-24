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

// управление моторами
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
namespace movingcore {
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
    protected: uint8_t SPEED_SET;
    public: void init(uint8_t speed) override { SPEED_SET = speed; }
};

/// @brief Релейный регулятор по левому датчику
static class RelayL : public Relay {
    protected: void calc() const override {
        bool L = lineL.on();
        motors::setSpeeds(L ? 0 : SPEED_SET, L ? SPEED_SET : 0);
    }
} relay_l;

/// @brief Релейный регулятор по правому датчику
static class RelayR : public Relay {
    protected: void calc() const override {
        bool R = lineR.on();
        motors::setSpeeds(R ? SPEED_SET : 0, R ? 0 : SPEED_SET);
    }
} relay_r;

/// @brief Релейный регулятор по двум датчикам
static class RelayLR : public Relay {
    protected: void calc() const override {
        bool L = lineL.on(), R = lineR.on();
        motors::setSpeeds((L > R) ? 0 : SPEED_SET, (L < R) ? 0 : SPEED_SET);
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
        BASE_SPEED = speed * 0.6;
        KP = 0.2;
    }
} proportional;

enum REGULATORS {
    RELAY_LR = 0,
    RELAY_L = 1,
    RELAY_R = 2,
    PROPORTIONAL = 3,
};

Regulator* factory(enum REGULATORS type, uint8_t speed) {
    static Regulator* regulators[]{
        &relay_lr,  // RELAY_LR
        &relay_l,  // RELAY_L        
        &relay_r,  // RELAY_R
        &proportional,  // PROPORTIONAL
    };

    Regulator* ret = regulators[type];
    ret->init(speed);
    return ret;
}

}


static void go_with_distance(hardware::DistanceSensor& sensor, uint8_t distance, int8_t speed, bool invert) {
    if (invert) speed *= -1;
    motors::setSpeeds(speed, speed);
    while ((sensor.read() > distance) ^ invert) motors::spin();
    goHold();
}
}


void setMotorsTime(int8_t speed_L, int8_t speed_R, uint32_t runtime, bool stop_at_exit) {
    runtime += millis();
    motors::setSpeeds(speed_L, speed_R);
    while (millis() < runtime) motors::spin();
    if (!stop_at_exit) return;
    motorL.setPWM(0);
    motorR.setPWM(0);
}

void goHold(uint32_t timeout) { setMotorsTime(0, 0, timeout); }

void go_1000_ticks(uint8_t speed) { motors::setForTicks(speed, 1000, speed, 1000); }

// Переводной макрос из ММ пути в тики энкодера
#define MM2TICKS(mm) ( (int32_t)(mm) * 1000L / (int32_t)(PARAMS::MM_IN_1000_TICKS) ) // .. где мой constexpr, Ардуино!!? где С++11???

void goDirect(int32_t distance_mm, uint8_t speed) {
    int32_t ticks = MM2TICKS(distance_mm);
    motors::setForTicks(speed, ticks, speed, ticks);
}

void goToWall(hardware::DistanceSensor& sensor, uint8_t wall_dist_cm, uint8_t speed) {
    movingcore::go_with_distance(sensor, wall_dist_cm, speed, false);
}

void goToWall(uint8_t wall_dist_cm, uint8_t speed) { goToWall(*robot.dist_front, wall_dist_cm, speed); }

void goBackWall(hardware::DistanceSensor& sensor, uint8_t wall_dist_cm, uint8_t speed) {
    movingcore::go_with_distance(sensor, wall_dist_cm, speed, true);
}

void goBackWall(uint8_t wall_dist_cm, uint8_t speed) { goBackWall(*robot.dist_front, wall_dist_cm, speed); }

void turnAngle(int16_t a, uint8_t speed) {
    int32_t ticks = MM2TICKS((int32_t) a * PARAMS::TRACK * M_PI / 360.0);
    motors::setForTicks(speed, ticks, speed, -ticks);
}

void golineTime(uint32_t runtime, uint8_t speed) {
    runtime += millis();

    using namespace movingcore::line;

    Regulator* reg = factory(PROPORTIONAL, speed);

    while (millis() < runtime) {
        reg->update();
    }

    goHold();
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
