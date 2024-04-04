#include "env.hpp"


using namespace mosh::hardware;
using namespace mosh::env;

LineSensor lineL(LINE_L, L_LINE, L_FIELD);
LineSensor lineR(LINE_R, R_LINE, R_FIELD);

NoDistanceSensor __no_sensor;
IrSensorSharp ir0(IR_0);
IrSensorSharp ir1(IR_1);
UltraSonic us(US_ECHO, US_TRIG);

MotorEncoder motorL(mosh::hardware::__l_int, MOTOR_L_INVERT, MOTOR_L_SPEED, MOTOR_L_DIR, MOTOR_L_ENC_A, MOTOR_L_ENC_B);
MotorEncoder motorR(mosh::hardware::__r_int, MOTOR_R_INVERT, MOTOR_R_SPEED, MOTOR_R_DIR, MOTOR_R_ENC_A, MOTOR_R_ENC_B);

void mosh::hardware::__l_int() { motorL.enc(); }
void mosh::hardware::__r_int() { motorR.enc(); }


void setDistL(DistanceSensor& sensor) { robot.dist_left = &sensor; }

void setDistR(DistanceSensor& sensor) { robot.dist_right = &sensor; }

void setDistF(DistanceSensor& sensor) { robot.dist_front = &sensor; }

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
