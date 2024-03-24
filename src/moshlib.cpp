#include "MoshLib.hpp"


// КОМПОНЕНТЫ

hardware::LineSensor lineL(pin::L_SENSOR, PARAMS::L_LINE, PARAMS::L_FIELD);
hardware::LineSensor lineR(pin::R_SENSOR, PARAMS::R_LINE, PARAMS::R_FIELD);

hardware::IrSensorSharp irL(pin::L_PIN);
hardware::IrSensorSharp irR(pin::R_PIN);

hardware::UltraSonic us(pin::PIN_ECHO, pin::PIN_TRIG);

hardware::MotorEncoder motorL(hardware::__l_int, pin::ML_INVERT, pin::ML_SPEED, pin::ML_DIR, pin::ML_ENC_A, pin::ML_ENC_B);
hardware::MotorEncoder motorR(hardware::__r_int, pin::MR_INVERT, pin::MR_SPEED, pin::MR_DIR, pin::MR_ENC_A, pin::MR_ENC_B);


void hardware::__l_int() { motorL.enc(); }
void hardware::__r_int() { motorR.enc(); }

// управление моторами
namespace motors
{
    void reset() {
        motorL.reset();
        motorR.reset();
    }

    void spin() {
        motorL.spin();
        motorR.spin();
    }

    void setSpeeds(int8_t left, int8_t right) {
        motorL.setSpeed(left);
        motorR.setSpeed(right);
    }

    void setForTicks(int8_t speed_L, int32_t ticks_L, int8_t speed_R, int32_t ticks_R) {
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

        goHold();
    }
} // namespace motors

void setMotorsTime(int8_t speed_L, int8_t speed_R, uint32_t runtime, bool stop_at_exit) {
    runtime += millis();
    motors::setSpeeds(speed_L, speed_R);
    while (millis() < runtime) motors::spin();
    if (!stop_at_exit) return;
    motorL.setPWM(0);
    motorR.setPWM(0);
}

void goHold(uint32_t timeout) { setMotorsTime(0, 0, timeout); }

void go_1000_ticks(uint8_t speed) { motors::setForTicks(speed, 1000, speed, 1000); }

// Переводной макрос из ММ пути в тики энкодера
#define MM2TICKS(mm) ( (int32_t)(mm) * 1000L / (int32_t)(PARAMS::MM_IN_1000_TICKS) ) // .. где мой constexpr, Ардуино!!? где С++11???

void goDirect(int32_t distance_mm, uint8_t speed) {
    int32_t ticks = MM2TICKS(distance_mm);
    motors::setForTicks(speed, ticks, speed, ticks);
}

void turnAngle(int16_t a, uint8_t speed) {
    int32_t ticks = MM2TICKS((int32_t)a * PARAMS::TRACK * M_PI / 360.0);
    motors::setForTicks(speed, ticks, speed, -ticks);
}

// ТЕСТИРОВАНИЕ

void test::lines(uint16_t& ret_L, uint16_t& ret_R, uint32_t timeout) {
    uint16_t i;
    uint32_t sum_L = 0, sum_R = 0;
    timeout += millis();

    for (i = 0; millis() < timeout; i++) {
        sum_L += lineL.readRaw();
        sum_R += lineR.readRaw();
    }

    ret_L = sum_L / i;
    ret_R = sum_R / i;
}


/**
 * @brief Изменение скорости регулятором
 * @param motor
 * @param dir
 */
static void test_motors_speed(hardware::MotorEncoder* motor, int8_t dir) {
    uint32_t timer = millis() + 120;

    motor->reset();

    for (int16_t speed = -PARAMS::MAX_DELTA_TICKS; speed <= PARAMS::MAX_DELTA_TICKS; speed++) {
        motor->setSpeed((PARAMS::MAX_DELTA_TICKS - abs(speed)) * dir);
        while (millis() < timer) motor->spin();
        timer += 120;
    }
    motor->setPWM(0);
    delay(500);
}

void test::motors_accel() {
    hardware::MotorEncoder* _motors[]{ &motorL, &motorR };

    for (hardware::MotorEncoder* m : _motors) {
        test_motors_speed(m, 1);
        test_motors_speed(m, -1);
    }
}

void test::motors_pwm() {
    hardware::MotorEncoder* _motors[]{ &motorL, &motorR };

    for (hardware::MotorEncoder* motor : _motors) {
        for (int16_t i = -255; i < 256; i++) {
            motor->setPWM(255 - abs(i));
            motor->setDir(i > 0);
            delay(2 * 2000 / 256);
        }
        motor->setPWM(0);
        delay(500);
    }
}
