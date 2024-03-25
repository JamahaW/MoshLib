#include "MoshLib.hpp"

#include "core/move.hpp"
#include "core/quit.hpp"
#include "core/enviroment.hpp"

using namespace mosh::core;
using namespace mosh::hardware;

static void run(const move::Mover& mover, const quit::Quiter& quiter, bool hold_at_end = true) {
    while (quiter.tick()) mover.update();
    if (hold_at_end) goHold();
    motorL.setPWM(0);
    motorR.setPWM(0);
}

static move::Mover* getLineRegulator(LINE_REGULATORS type, uint8_t speed) {
    using namespace mosh::core::move;
    switch (type) {
        case LINE_REGULATORS::RELAY_L: return new RelayLineSingle(speed, RelayLineSingle::LINE_LEFT);
        case LINE_REGULATORS::RELAY_R: return new RelayLineSingle(speed, RelayLineSingle::LINE_RIGHT);
        case LINE_REGULATORS::RELAY_LR: return new RelayLineBoth(speed);
        case LINE_REGULATORS::PROP: return new ProportionalLineRegulator(speed);
        default: return new Mover;
    }
}

void distSensorL(DistanceSensor& sensor) { robot.dist_left = &sensor; }

void distSensorR(DistanceSensor& sensor) { robot.dist_right = &sensor; }

void distSensorF(DistanceSensor& sensor) { robot.dist_front = &sensor; }

void goTime(uint32_t runtime, int8_t speed_left, int8_t speed_right, bool __hold_at_end) {
    using namespace mosh::core;
    run(move::KeepSpeed(speed_left, speed_right), quit::OnTimer(runtime), __hold_at_end);
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

void goWallFront(DistanceSensor& sensor, uint8_t wall_dist_cm, uint8_t speed) {
    using namespace mosh::core;
    run(move::KeepSpeed(speed, speed), quit::IfDistanceSensorRead(sensor, wall_dist_cm, quit::IfDistanceSensorRead::GREATER));
}

void goWallFront(uint8_t distance, uint8_t speed) { goWallFront(*robot.dist_front, distance, speed); }

void goWallBack(DistanceSensor& sensor, uint8_t wall_dist_cm, uint8_t speed) {
    using namespace mosh::core;
    run(move::KeepSpeed(speed, speed), quit::IfDistanceSensorRead(sensor, wall_dist_cm, quit::IfDistanceSensorRead::LESS));
}

void goWallBack(uint8_t distance, uint8_t speed) { goWallBack(*robot.dist_front, distance, speed); }

void turnAngle(int16_t a, uint8_t speed) {
    int32_t ticks = MM2TICKS((int32_t) a * PARAMS::TRACK * M_PI / 360.0);
    motors::setForTicks(speed, ticks, speed, -ticks);
}

void lineReg(enum LINE_REGULATORS default_regulator) { robot.line_follow_regulator = default_regulator; }

void goLineTime(enum LINE_REGULATORS type, uint32_t runtime, uint8_t speed) {
    using namespace mosh::core;
    move::Mover* mover = getLineRegulator(type, speed);
    run(*mover, quit::OnTimer(runtime));
    delete mover;
}

void goLineTime(uint32_t runtime, uint8_t speed) { goLineTime(robot.line_follow_regulator, runtime, speed); }

void goLwallTime(uint8_t distance, uint32_t runtime, uint8_t speed) {
    using namespace mosh::core;
    using namespace mosh::core::move;
    run(MoveAlongWall(speed, distance, MoveAlongWall::DIST_LEFT), quit::OnTimer(runtime));
}

void goRwallTime(uint8_t distance, uint32_t runtime, uint8_t speed) {
    using namespace mosh::core;
    using namespace mosh::core::move;
    run(MoveAlongWall(speed, distance, MoveAlongWall::DIST_RIGHT), quit::OnTimer(runtime));
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
static void test_motors_speed(MotorEncoder* motor, int8_t dir) {
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
    MotorEncoder* _motors[]{ &motorL, &motorR };

    for (MotorEncoder* m : _motors) {
        test_motors_speed(m, 1);
        test_motors_speed(m, -1);
    }
}

void test::motorsPWM() {
    MotorEncoder* _motors[]{ &motorL, &motorR };

    for (MotorEncoder* motor : _motors) {
        for (int16_t i = -255; i < 256; i++) {
            motor->setPWM(255 - abs(i));
            motor->setDir(i > 0);
            delay(2 * 2000 / 256);
        }
        motor->setPWM(0);
        delay(500);
    }
}
