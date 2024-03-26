#include "move.hpp"
#include "core/enviroment.hpp"


using namespace mosh::control;

void Mover::tick() const {};

void Mover::update() const {
    tick();
    motors::spin();
}

const Mover& KeepSpeed::init(int8_t speed_left, int8_t speed_right) {
    motors::setSpeeds(speed_left, speed_right);
    return *this;
}

void ProportionalLineRegulator::tick() const {
    int8_t u = (lineL() - lineR()) * KP;
    motors::setSpeeds(BASE_SPEED - u, BASE_SPEED + u);
}

const Mover& ProportionalLineRegulator::init(int8_t speed) {
    BASE_SPEED = speed * 0.7;
    return *this;
}

RelayLineSingle::RelayLineSingle(SENSOR sensor_dir)
    : __dir(sensor_dir), sensor((__dir == LINE_LEFT) ? &lineL : (__dir == LINE_RIGHT) ? &lineR : nullptr) {}

const Mover& RelayLineSingle::init(int8_t speed) {
    int8_t second = speed * -0.3; // TODO вынести коэф
    SPEED_A = SPEED_B = speed;
    if (__dir == SENSOR::LINE_LEFT) SPEED_A = second;
    if (__dir == SENSOR::LINE_RIGHT) SPEED_B = second;
    return *this;
}

void RelayLineSingle::tick() const {
    bool on = sensor->on();
    motors::setSpeeds(on ? SPEED_A : SPEED_B, on ? SPEED_B : SPEED_A);
}

const Mover& RelayLineBoth::init(int8_t speed) {
    SPEED = speed;
    SECOND = speed * -0.3; // TODO выгесьт коэф
    return *this;
}

void RelayLineBoth::tick() const {
    bool L = lineL.on(), R = lineR.on();
    motors::setSpeeds((L > R) ? SECOND : SPEED, (L < R) ? SECOND : SPEED);
}

MoveAlongWall::MoveAlongWall(uint8_t distance, POS pos) :
    TARGET(distance), k(1.2 * (int) pos), sensor((pos == DIST_LEFT) ? robot.dist_left : (pos == DIST_RIGHT) ? robot.dist_right : &no_sensor) {}

const Mover& MoveAlongWall::init(int8_t speed) {
    SPEED = speed;
    return *this;
}

void MoveAlongWall::tick() const {
    int16_t u = k * float(TARGET - sensor->read()) * SPEED / (float) TARGET;
    motors::setSpeeds(SPEED - u, SPEED + u);
}

