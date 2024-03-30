#include "enviroment.hpp"


mosh::hardware::LineSensor lineL(LINE_L, L_LINE, L_FIELD);
mosh::hardware::LineSensor lineR(LINE_R, R_LINE, R_FIELD);

mosh::hardware::NoDistanceSensor no_sensor;
mosh::hardware::IrSensorSharp ir0(IR_0);
mosh::hardware::IrSensorSharp ir1(IR_1);
mosh::hardware::UltraSonic us(US_ECHO, US_TRIG);

mosh::hardware::MotorEncoder motorL(mosh::hardware::__l_int, MOTOR_L_INVERT, MOTOR_L_SPEED, MOTOR_L_DIR, MOTOR_L_ENC_A, MOTOR_L_ENC_B);
mosh::hardware::MotorEncoder motorR(mosh::hardware::__r_int, MOTOR_R_INVERT, MOTOR_R_SPEED, MOTOR_R_DIR, MOTOR_R_ENC_A, MOTOR_R_ENC_B);

void mosh::hardware::__l_int() { motorL.enc(); }
void mosh::hardware::__r_int() { motorR.enc(); }

RobotConfig robot;

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