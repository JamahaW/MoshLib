#include "motor.hpp"


using namespace mosh::hardware;

MotorL298N& MotorL298N::pinout(uint8_t speed, uint8_t dir, bool invert) {
    SPEED = speed;
    DIR = dir;
    INVERT = invert;
    pinMode(SPEED, OUTPUT);
    pinMode(DIR, OUTPUT);
    return *this;
}

void MotorL298N::setDir(bool reverse) { digitalWrite(DIR, reverse ^ INVERT); }

void MotorL298N::setPWM(uint8_t pwm) { analogWrite(SPEED, pwm); }

void MotorL298N::setDirPWM(int16_t dir_pwm) {
    setDir(dir_pwm < 0);
    dir_pwm = constrain(dir_pwm, -255, 255);
    setPWM(abs(dir_pwm));
}

MotorEncoder& MotorEncoder::encoder(uint8_t enc_a, uint8_t enc_b, bool invert) {
    ENC_INV = invert;
    pinMode(enc_a, INPUT);
    pinMode(ENC_B = enc_b, INPUT);
    attachInterrupt(digitalPinToInterrupt(enc_a), handler, FALLING);
    return *this;
}

MotorEncoder::MotorEncoder(void(*encoder_handler)(void)) : handler(encoder_handler) {}

void MotorEncoder::__enc() { position += (digitalRead(ENC_B) ^ ENC_INV) ? (1) : (-1); }

void MotorEncoder::reset() { position = next_pos = d_pos = timer = 0; }

void MotorEncoder::setSpeed(int16_t dtick) { d_pos = constrain(dtick, -MAX_DELTA_TICKS, MAX_DELTA_TICKS); }

void MotorEncoder::spin() {
    setDirPWM(MOTOR_ENCODER_SPEED_KP * (next_pos - position));
    if (millis() <= timer) return;
    timer = millis() + SPIN_PERIOD_MS;
    next_pos += d_pos;
    return;
}

/// @brief Обработчик левого мотора
static void __l_int() { motorL.__enc(); }

/// @brief Обработчик правого мотора
static void __r_int() { motorR.__enc(); }

MotorEncoder motorL(__l_int);
MotorEncoder motorR(__r_int);


void MotorController::reset() {
    motorL.reset();
    motorR.reset();
}

void MotorController::spin() {
    motorL.spin();
    motorR.spin();
}

void MotorController::setSpeed(int16_t left, int16_t right) {
    motorL.setSpeed(left);
    motorR.setSpeed(right);
}

void MotorController::setDirPWM(int16_t dirpwm_left, int16_t dirpwm_right) {
    motorL.setDirPWM(dirpwm_left);
    motorR.setDirPWM(dirpwm_right);
}

MotorController motors;


