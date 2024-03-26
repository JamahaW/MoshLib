#include "move.hpp"
#include "core/enviroment.hpp"


using namespace mosh::control;

void Mover::tick() const {};

void Mover::update() const {
    tick();
    motors::spin();
}

KeepSpeed::KeepSpeed(int8_t speed_left, int8_t speed_right) { motors::setSpeeds(speed_left, speed_right); }

void LineProportional::tick() const {
    int8_t u = (lineL() - lineR()) * KP;
    motors::setSpeeds(BASE_SPEED - u, BASE_SPEED + u);
}

LineProportional::LineProportional(int8_t speed) : BASE_SPEED(speed * 0.7) {}

LineRelaySingle::LineRelaySingle(SENSOR dir, int8_t speed) :
    SPEED_A((dir == LINE_LEFT) ? speed : speed * -0.3),
    SPEED_B((dir == LINE_RIGHT) ? speed : speed * -0.3),
    sensor((dir == LINE_LEFT) ? &lineL : (dir == LINE_RIGHT) ? &lineR : nullptr) {}

void LineRelaySingle::tick() const {
    bool on = sensor->on();
    motors::setSpeeds(on ? SPEED_A : SPEED_B, on ? SPEED_B : SPEED_A);
}

// TODO выгесьт коэф 
LineRelayBoth::LineRelayBoth(int8_t speed) :
    SPEED(speed),
    SECOND(speed * -0.3) {}

void LineRelayBoth::tick() const {
    bool L = lineL.on(), R = lineR.on();
    motors::setSpeeds((L > R) ? SECOND : SPEED, (L < R) ? SECOND : SPEED);
}

MoveAlongWall::MoveAlongWall(uint8_t distance, POS pos, int8_t speed) :
    SPEED(speed),
    TARGET(distance), k(1.2 * (int) pos), // вынести коэф
    sensor((pos == DIST_LEFT) ? robot.dist_left : (pos == DIST_RIGHT) ? robot.dist_right : &no_sensor) {}

void MoveAlongWall::tick() const {
    int16_t u = k * float(TARGET - sensor->read()) * SPEED / (float) TARGET;
    motors::setSpeeds(SPEED - u, SPEED + u);
}

