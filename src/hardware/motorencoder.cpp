#include "motorencoder.hpp"


using namespace mosh::hardware;

MotorEncoder::MotorEncoder(void (*encoder_handler) (void), const bool inverted, const uint8_t pin_speed, const uint8_t pin_dir, const uint8_t pin_enc_a, const uint8_t pin_enc_b)
    : PIN_SPEED(pin_speed), PIN_DIR(pin_dir), PIN_ENC_B(pin_enc_b), INVERTED(inverted) {
    pinMode(PIN_SPEED, OUTPUT);
    pinMode(PIN_DIR, OUTPUT);
    pinMode(PIN_ENC_B, INPUT);
    pinMode(pin_enc_a, INPUT);
    attachInterrupt(digitalPinToInterrupt(pin_enc_a), encoder_handler, FALLING);
}

void MotorEncoder::enc() { position += (digitalRead(PIN_ENC_B) ^ INVERTED) ? (1) : (-1); }

void MotorEncoder::reset() { position = next_pos = speed_set = speed = next_pos_timer = 0; }

void MotorEncoder::setDir(bool backward) { digitalWrite(PIN_DIR, backward ^ INVERTED); }

void MotorEncoder::setPWM(uint8_t pwm) { analogWrite(PIN_SPEED, pwm); }

void MotorEncoder::setDirPWM(int16_t power) {
    setDir(power < 0);
    power = abs(power);
    setPWM(constrain(power, 0, 255));
}

void MotorEncoder::setSpeed(int16_t dtick) { speed = speed_set = constrain(dtick, -PARAMS::MAX_DELTA_TICKS, PARAMS::MAX_DELTA_TICKS); }

void MotorEncoder::spin() {
    setDirPWM(KP_SPEED * (next_pos - position));
    if (millis() <= next_pos_timer) return;
    next_pos_timer = millis() + PARAMS::SPIN_PERIOD_MS;
    next_pos += speed;
    return;
}
