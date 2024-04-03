#include "MoshLib.hpp"
#include "core/run.hpp"


using mosh::hardware::DistanceSensor;
using mosh::hardware::MotorEncoder;

using namespace mosh::control;
using namespace mosh;


void setDistL(DistanceSensor& sensor) { robot.dist_left = &sensor; }

void setDistR(DistanceSensor& sensor) { robot.dist_right = &sensor; }

void setDistF(DistanceSensor& sensor) { robot.dist_front = &sensor; }

// // TODO доделать!!
// void setLineReg(enum LINE_REGULATORS default_regulator) {
//     robot.line_follow_regulator = default_regulator;
// }



void goTime(uint32_t runtime, int8_t speed_left, int8_t speed_right, bool __hold_at_end) {
    run::base(KeepSpeed(speed_left, speed_right), OnTimer(runtime), __hold_at_end);
}

void goTime(uint32_t runtime, int8_t speed) { goTime(runtime, speed, speed); }

void goHold(uint32_t timeout) { goTime(timeout, 0, 0, false); }

void goTick(int32_t ticks, uint8_t speed) { run::base(Sync(speed), DistanceMoved(ticks, ticks, false)); }

void goDist(int32_t distance_mm, int16_t speed) {
    if (distance_mm < 0) speed *= -1;
    run::base(Sync(speed), DistanceMoved(distance_mm));
}

void goCross(uint8_t crosses, bool found_line, int16_t speed_PWMDIR) { run::base(Sync(speed_PWMDIR), LineFound(LineFound::LINE_BOTH, crosses, found_line)); }



void turnAngle(int16_t a, int16_t speed) {
    int32_t dist = (int32_t) a * (TRACK_SIZE_MM * M_PI / 360.0);
    run::base(Sync(speed * ((a > 0) - 1), -speed * ((a > 0) - 1), 1, -1), DistanceMoved(dist, -dist));
}

void alignLine(uint32_t duration) { run::time(LineProp(0), duration); }

static void __turn_line(int8_t crosses, int16_t speed, LineFound::MODE dir) {
    run::base(Sync(-speed, speed, -1, 1), LineFound(dir, crosses, false));
    alignLine();
}

void turnLineL(int8_t crosses, int16_t speed) { __turn_line(crosses, speed, LineFound::LINE_LEFT); }

void turnLineR(int8_t crosses, int16_t speed) { __turn_line(crosses, -speed, LineFound::LINE_RIGHT); }



void wallFront(DistanceSensor& sensor, uint8_t wall_dist_cm, uint8_t speed) {
    run::base(Sync(speed), DistanceRead(sensor, wall_dist_cm, DistanceRead::GREATER));
}

void wallFront(uint8_t distance, uint8_t speed) { wallFront(*robot.dist_front, distance, speed); }

void wallBack(DistanceSensor& sensor, uint8_t wall_dist_cm, int16_t speed) {
    run::base(Sync(-speed), DistanceRead(sensor, wall_dist_cm, DistanceRead::LESS));
}

void wallBack(uint8_t distance, uint8_t speed) { wallBack(*robot.dist_front, distance, speed); }

void wallTimeL(uint8_t distance, uint32_t runtime, uint8_t speed) {
    run::time(MoveAlongWall(distance, MoveAlongWall::DIST_LEFT, speed), runtime);
}

void wallTimeR(uint8_t distance, uint32_t runtime, uint8_t speed) {
    run::time(MoveAlongWall(distance, MoveAlongWall::DIST_RIGHT, speed), runtime);
}



// void lineTime(uint32_t runtime, uint8_t speed) {
//     // lineTime(robot.line_follow_regulator, runtime, speed);
// }

void lineTimeL(uint32_t runtime, uint8_t speed) { run::time(LineRelay(LineRelay::LINE_LEFT, speed), runtime); }

void lineTimeR(uint32_t runtime, uint8_t speed) { run::time(LineRelay(LineRelay::LINE_RIGHT, speed), runtime); }

void lineTimeLR(uint32_t runtime, uint8_t speed) { run::time(LineRelay2(speed), runtime); }

void lineTimeP(uint32_t runtime, uint8_t speed) { run::time(LineProp(speed), runtime); }


void lineCrossP(uint8_t crosses, uint8_t speed) { run::base(LineProp(speed), LineFound(LineFound::LINE_BOTH, crosses, false)); }

void lineDistP(int32_t distance, uint8_t speed) { run::base(LineProp(speed), DistanceMoved(distance)); }

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
