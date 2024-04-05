#include "move.hpp"
#include "hardware/motorencoder.hpp"
#include "hardware/robotconfig.hpp"


using namespace mosh::control;

void Mover::tick() const {};

KeepSpeed::KeepSpeed(int8_t speed_left, int8_t speed_right) { motors.setSpeed(speed_left, speed_right); }

KeepSpeed::KeepSpeed(int8_t speed) : KeepSpeed(speed, speed) {}

void mosh::control::KeepSpeed::tick() const { motors.motorsSpin(); }

void LineProp::tick() const {
    int8_t u = (lineL() - lineR()) * KP;
    motors.setSpeed(BASE_SPEED - u, BASE_SPEED + u);
    motors.motorsSpin();
}

LineProp::LineProp(int8_t speed) : BASE_SPEED(speed * 0.7) {}

LineRelay::LineRelay(SENSOR dir, int8_t speed) :
    SPEED_A((dir == LINE_LEFT) ? speed : speed * -0.3),
    SPEED_B((dir == LINE_RIGHT) ? speed : speed * -0.3),
    sensor((dir == LINE_LEFT) ? &lineL : (dir == LINE_RIGHT) ? &lineR : nullptr) {}

void LineRelay::tick() const {
    bool on = sensor->on();
    motors.setSpeed(on ? SPEED_A : SPEED_B, on ? SPEED_B : SPEED_A);
    motors.motorsSpin();
}

// TODO выгесьт коэф 
LineRelay2::LineRelay2(int8_t speed) :
    SPEED(speed),
    SECOND(speed * -0.3) {}

void LineRelay2::tick() const {
    bool L = lineL.on(), R = lineR.on();
    motors.setSpeed((L > R) ? SECOND : SPEED, (L < R) ? SECOND : SPEED);
    motors.motorsSpin();
}

MoveAlongWall::MoveAlongWall(uint8_t distance, POS pos, int8_t speed) :
    SPEED(speed),
    TARGET(distance), k(1.2 * (int) pos), // вынести коэф
    sensor((pos == DIST_LEFT) ? conf.dist_left : (pos == DIST_RIGHT) ? conf.dist_right : &__no_sensor) {}

void MoveAlongWall::tick() const {
    int16_t u = k * float(TARGET - sensor->read()) * SPEED / (float) TARGET;
    motors.setSpeed(SPEED - u, SPEED + u);
    motors.motorsSpin();
}

Sync::Sync(int16_t fact_l, int16_t fact_r, int8_t kl, int8_t kr) :
    FACT_L(fact_l), FACT_R(fact_r), KL(kl), KR(kr), SIG_L(SIGN(KL)), SIG_R(KR) {
    motors.reset();
}

Sync::Sync(int16_t fact) : Sync(fact, fact, 1, 1) {}

void Sync::tick() const { // TODO разобоаться как сделать не ШИМ
    int16_t u = (KL * motorL.position - KR * motorR.position) * ENV_PARAMS::SYNC_K;
    motors.setDirPWM(FACT_L - u * SIG_L, FACT_R + u * SIG_R);
}
