#include "MoshLib.hpp"
#include "core/run.hpp"
#include "util/tools.hpp"


using mosh::hardware::DistanceSensor;
using mosh::hardware::MotorEncoder;

using namespace mosh::control;
using namespace mosh;


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
    int32_t dist = (int32_t) a * (conf.__track_size * M_PI / 360.0);
    run::base(Sync(a > 0 ? speed : -speed, a > 0 ? -speed : speed, 1, -1), DistanceMoved(dist, -dist));
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

void wallFront(uint8_t distance, uint8_t speed) { wallFront(*conf.dist_front, distance, speed); }

void wallBack(DistanceSensor& sensor, uint8_t wall_dist_cm, int16_t speed) {
    run::base(Sync(-speed), DistanceRead(sensor, wall_dist_cm, DistanceRead::LESS));
}

void wallBack(uint8_t distance, uint8_t speed) { wallBack(*conf.dist_front, distance, speed); }

void wallTimeL(uint8_t distance, uint32_t runtime, uint8_t speed) {
    run::time(MoveAlongWall(distance, LEFT, speed), runtime);
}

void wallTimeR(uint8_t distance, uint32_t runtime, uint8_t speed) {
    run::time(MoveAlongWall(distance, RIGHT, speed), runtime);
}



void lineTimeL(uint32_t runtime, uint8_t speed) { run::time(LineRelay(LEFT, speed), runtime); }

void lineTimeR(uint32_t runtime, uint8_t speed) { run::time(LineRelay(RIGHT, speed), runtime); }

void lineTimeLR(uint32_t runtime, uint8_t speed) { run::time(LineRelay2(speed), runtime); }

void lineTimeP(uint32_t runtime, uint8_t speed) { run::time(LineProp(speed), runtime); }


void lineCrossP(uint8_t crosses, uint8_t speed) { run::base(LineProp(speed), LineFound(LineFound::LINE_BOTH, crosses, false)); }

void lineDistP(int32_t distance, uint8_t speed) { run::base(LineProp(speed), DistanceMoved(distance)); }
