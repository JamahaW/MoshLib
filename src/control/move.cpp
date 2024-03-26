#include "move.hpp"
#include "core/enviroment.hpp"


using namespace mosh::control;

void Mover::tick() const {};

void Mover::update() const {
    tick();
    motors::spin();
}

Mover::~Mover() {}

KeepSpeed::KeepSpeed(int8_t speed_left, int8_t speed_right) { motors::setSpeeds(speed_left, speed_right); }

ProportionalLineRegulator::ProportionalLineRegulator(uint8_t speed) : BASE_SPEED(speed * 0.7) {}

void ProportionalLineRegulator::tick() const {
    int8_t u = (lineL() - lineR()) * KP;
    motors::setSpeeds(BASE_SPEED - u, BASE_SPEED + u);
}

RelayLineSingle::RelayLineSingle(uint8_t speed, enum SENSOR sensor_dir) {
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

void RelayLineSingle::tick() const {
    bool on = sensor->on();
    motors::setSpeeds(on ? SPEED_A : SPEED_B, on ? SPEED_B : SPEED_A);
}

RelayLineBoth::RelayLineBoth(uint8_t speed) : SPEED(speed), SECOND((int8_t) -speed * 0.3) {}

void RelayLineBoth::tick() const {
    bool L = lineL.on(), R = lineR.on();
    motors::setSpeeds((L > R) ? SECOND : SPEED, (L < R) ? SECOND : SPEED);
}

MoveAlongWall::MoveAlongWall(int8_t speed, uint8_t target_distance_cm, POS direction) :
    SPEED(speed), TARGET(target_distance_cm), k(1.2 * (int) direction) {
    switch (direction) {
        case POS::DIST_LEFT: sensor = robot.dist_left; break;
        case POS::DIST_RIGHT: sensor = robot.dist_right; break;
        default: sensor = &no_sensor; break;
    }
}

void MoveAlongWall::tick() const {
    int16_t u = k * float(TARGET - sensor->read()) * SPEED / (float) TARGET;
    motors::setSpeeds(SPEED - u, SPEED + u);
}