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

void MotorEncoder::reset() { position = next_pos = speed_set = speed = timer = 0; }

void MotorEncoder::setDir(bool backward) { digitalWrite(PIN_DIR, backward ^ INVERTED); }

void MotorEncoder::setPWM(uint8_t pwm) { analogWrite(PIN_SPEED, pwm); }

void MotorEncoder::setDirPWM(int16_t power) {
    setDir(power < 0);
    power = abs(power);
    setPWM(constrain(power, 0, 255));
}

void MotorEncoder::setSpeed(int16_t dtick) { speed = speed_set = constrain(dtick, -MAX_DELTA_TICKS, MAX_DELTA_TICKS); }

void MotorEncoder::spin() {
    setDirPWM(KP_SPEED * (next_pos - position));
    if (millis() <= timer) return;
    timer = millis() + SPIN_PERIOD_MS;
    next_pos += speed;
    return;
}

/// @brief Обработчик левого мотора
static void __l_int() { motorL.enc(); }

/// @brief Обработчик правого мотора
static void __r_int() { motorR.enc(); }

MotorEncoder motorL(__l_int, MOTOR_L_INVERT, MOTOR_L_SPEED, MOTOR_L_DIR, MOTOR_L_ENC_A, MOTOR_L_ENC_B);
MotorEncoder motorR(__r_int, MOTOR_R_INVERT, MOTOR_R_SPEED, MOTOR_R_DIR, MOTOR_R_ENC_A, MOTOR_R_ENC_B);


void MotorController::reset() {
    motorL.reset();
    motorR.reset();
}

void MotorController::motorsSpin() {
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
