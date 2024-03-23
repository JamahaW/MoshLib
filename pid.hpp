#pragma once

#include <Arduino.h>


template <class T> struct PID {
    const T KP, KI, KD;
    uint8_t period;
    uint32_t next_calc = 0;
    T result = 0, integral = 0, prev_error = 0;

    PID(T kp, T ki, T kd, const float dt)
        : KP(kp), KI(ki* dt), KD(kd / dt), period(1000.0 * dt) {}

    const T calc(T error) {
        if (millis() >= next_calc) {
            next_calc = millis() + (uint32_t)period;
            integral += error * KI;
            result = error * KP + integral + (prev_error - error) * KD;
        }
        return result;
    }

    void reset() {
        integral = 0;
        prev_error = 0;
    }
};

