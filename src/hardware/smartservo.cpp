#include "smartservo.hpp"


using namespace mosh::hardware;

void SmartServo::use() const { servo.attach(PIN); }

void SmartServo::disable() const { servo.detach(); }

void SmartServo::set(uint8_t angle) const { servo.write(last_angle = angle); }

void SmartServo::turn(uint8_t angle) const {
    if (AUTODETACH) {
        use();
        delay(SMARTSERVO_TURN_TIMEOUT);
    }

    uint8_t steps = abs(last_angle - angle);

    for (uint8_t i = 0; i < steps; i++) {
        set(map(i, 0, steps, last_angle, angle));
        delay(dt);
    }

    if (!AUTODETACH) return;

    delay(SMARTSERVO_TURN_TIMEOUT);
    disable();
}

SmartServo& SmartServo::pinout(uint8_t pin) {
    PIN = pin;
    return *this;
}

SmartServo& SmartServo::config(bool autodetach, uint8_t dt, uint8_t init_angle) {
    AUTODETACH = autodetach;
    this->dt = dt;
    use();
    set(last_angle = init_angle);
    return *this;
}

SmartServo servo1;
