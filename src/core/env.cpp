#include "env.hpp"


using namespace mosh::hardware;
using namespace mosh::env;

LineSensor lineL;
LineSensor lineR;

NoDistanceSensor __no_sensor;
IrSensorSharp ir0;
IrSensorSharp ir1;
UltraSonic us;

MotorEncoder motorL(mosh::hardware::__l_int, MOTOR_L_INVERT, MOTOR_L_SPEED, MOTOR_L_DIR, MOTOR_L_ENC_A, MOTOR_L_ENC_B);
MotorEncoder motorR(mosh::hardware::__r_int, MOTOR_R_INVERT, MOTOR_R_SPEED, MOTOR_R_DIR, MOTOR_R_ENC_A, MOTOR_R_ENC_B);

void mosh::hardware::__l_int() { motorL.enc(); }
void mosh::hardware::__r_int() { motorR.enc(); }

int32_t RobotController::mm2ticks(int32_t mm) { return mm * 1000L / mm_in_1000_ticks; }

void RobotController::motorsReset() {
    motorL.reset();
    motorR.reset();
}

void RobotController::motorsSpin() {
    motorL.spin();
    motorR.spin();
}

void RobotController::motorsSpeed(int16_t left, int16_t right) {
    motorL.setSpeed(left);
    motorR.setSpeed(right);
}

void RobotController::motorsDirPWM(int16_t dirpwm_left, int16_t dirpwm_right) {
    motorL.setDirPWM(dirpwm_left);
    motorR.setDirPWM(dirpwm_right);
}

RobotController mosh::env::robot;

void setDistL(DistanceSensor& sensor) { robot.dist_left = &sensor; }

void setDistR(DistanceSensor& sensor) { robot.dist_right = &sensor; }

void setDistF(DistanceSensor& sensor) { robot.dist_front = &sensor; }

void confTrack(uint8_t trasksize_mm) { robot.track_size = trasksize_mm; }

void confSpeedTick(uint8_t ticks) { robot.default_speed_tick = ticks; }

void confSpeedPWM(uint8_t pwm) { robot.default_speed_pwm = pwm; }

void confMM2Ticks(uint16_t mm_in_1000_ticks) { robot.mm_in_1000_ticks = mm_in_1000_ticks; }
