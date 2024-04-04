#include "env.hpp"

using namespace mosh::hardware;

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

mosh::env::RobotController robot;

// управление моторами [[[  // TODO ликвидировать!  ]]]
void motors::reset() {
    motorL.reset();
    motorR.reset();
}

void motors::spin() {
    motorL.spin();
    motorR.spin();
}

void motors::setSpeeds(int16_t left, int16_t right) {
    motorL.setSpeed(left);
    motorR.setSpeed(right);
}

void setDistL(DistanceSensor& sensor) { robot.dist_left = &sensor; }

void setDistR(DistanceSensor& sensor) { robot.dist_right = &sensor; }

void setDistF(DistanceSensor& sensor) { robot.dist_front = &sensor; }
