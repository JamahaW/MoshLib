#include "enviroment.hpp"

hardware::LineSensor lineL(pin::L_SENSOR, PARAMS::L_LINE, PARAMS::L_FIELD);
hardware::LineSensor lineR(pin::R_SENSOR, PARAMS::R_LINE, PARAMS::R_FIELD);

hardware::NoDistanceSensor no_sensor;
hardware::IrSensorSharp ir0(pin::IR_0);
hardware::IrSensorSharp ir1(pin::IR_1);
hardware::UltraSonic us(pin::PIN_ECHO, pin::PIN_TRIG);

hardware::MotorEncoder motorL(hardware::__l_int, pin::ML_INVERT, pin::ML_SPEED, pin::ML_DIR, pin::ML_ENC_A, pin::ML_ENC_B);
hardware::MotorEncoder motorR(hardware::__r_int, pin::MR_INVERT, pin::MR_SPEED, pin::MR_DIR, pin::MR_ENC_A, pin::MR_ENC_B);

void hardware::__l_int() { motorL.enc(); }
void hardware::__r_int() { motorR.enc(); }

RobotConfig robot;

// управление моторами [[[  TODO ликвидировать!  ]]]
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

void motors::setForTicks(int8_t speed_L, int32_t ticks_L, int8_t speed_R, int32_t ticks_R) {
    reset();
    setSpeeds(speed_L, speed_R);
    motorL.target = ticks_L;
    motorR.target = ticks_R;

    bool runL = true;
    bool runR = true;

    while (runL || runR) {
        runL = motorL.follow();
        runR = motorR.follow();
    }

    // goHold();
    setSpeeds(0, 0);
}
