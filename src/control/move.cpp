#include "move.hpp"
#include "core/enviroment.hpp"


using namespace mosh::control;

void Mover::tick() const {};

KeepSpeed::KeepSpeed(int8_t speed_left, int8_t speed_right) { motors::setSpeeds(speed_left, speed_right); }

KeepSpeed::KeepSpeed(int8_t speed) : KeepSpeed(speed, speed) {}

void mosh::control::KeepSpeed::tick() const { motors::spin(); }

void LineProp::tick() const {
    int8_t u = (lineL() - lineR()) * KP;
    motors::setSpeeds(BASE_SPEED - u, BASE_SPEED + u);
    motors::spin();
}

LineProp::LineProp(int8_t speed) : BASE_SPEED(speed * 0.7) {}

LineRelay::LineRelay(SENSOR dir, int8_t speed) :
    SPEED_A((dir == LINE_LEFT) ? speed : speed * -0.3),
    SPEED_B((dir == LINE_RIGHT) ? speed : speed * -0.3),
    sensor((dir == LINE_LEFT) ? &lineL : (dir == LINE_RIGHT) ? &lineR : nullptr) {}

void LineRelay::tick() const {
    bool on = sensor->on();
    motors::setSpeeds(on ? SPEED_A : SPEED_B, on ? SPEED_B : SPEED_A);
    motors::spin();
}

// TODO выгесьт коэф 
LineRelay2::LineRelay2(int8_t speed) :
    SPEED(speed),
    SECOND(speed * -0.3) {}

void LineRelay2::tick() const {
    bool L = lineL.on(), R = lineR.on();
    motors::setSpeeds((L > R) ? SECOND : SPEED, (L < R) ? SECOND : SPEED);
    motors::spin();
}

MoveAlongWall::MoveAlongWall(uint8_t distance, POS pos, int8_t speed) :
    SPEED(speed),
    TARGET(distance), k(1.2 * (int) pos), // вынести коэф
    sensor((pos == DIST_LEFT) ? robot.dist_left : (pos == DIST_RIGHT) ? robot.dist_right : &no_sensor) {}

void MoveAlongWall::tick() const {
    int16_t u = k * float(TARGET - sensor->read()) * SPEED / (float) TARGET;
    motors::setSpeeds(SPEED - u, SPEED + u);
    motors::spin();
}

Sync::Sync(int16_t fact_l, int16_t fact_r, int8_t kl, int8_t kr) :
    FACT_L(fact_l), FACT_R(fact_r), KL(kl), KR(kr), SIG_L(SIGN(KL)), SIG_R(KR) {
    motors::reset();
}

Sync::Sync(int16_t fact) : Sync(fact, fact, 1, 1) {}

void Sync::tick() const { // TODO разобоаться как сделать не ШИМ
    int16_t u = (KL * motorL.position - KR * motorR.position) * PARAMS::SYNC_K;
    motorL.setDirPWM(FACT_L - u * SIG_L);
    motorR.setDirPWM(FACT_R + u * SIG_R);
}
