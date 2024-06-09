#include "tests.hpp"
#include "hardware/motor.hpp"
#include "hardware/linesensor.hpp"


using namespace mosh::test;
using namespace mosh::hardware;

// ТЕСТИРОВАНИЕ
void lines(uint16_t& ret_L, uint16_t& ret_R, uint32_t timeout) {
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

static void test_motors_speed(MotorEncoder* motor, int8_t dir) {
    uint32_t timer = millis() + 120;

    motor->reset();

    for (int16_t speed = -MAX_DELTA_TICKS; speed <= MAX_DELTA_TICKS; speed++) {
        motor->setSpeed((MAX_DELTA_TICKS - abs(speed)) * dir);
        while (millis() < timer) motor->spin();
        timer += 120;
    }
    motor->setPWM(0);
    delay(500);
}


void motorsAccel() {
    MotorEncoder* _motors[]{ &motorL, &motorR };

    for (MotorEncoder* m : _motors) {
        test_motors_speed(m, 1);
        test_motors_speed(m, -1);
    }
}

void motorsPWM() {
    MotorEncoder* _motors[]{ &motorL, &motorR };

    for (MotorEncoder* motor : _motors) {
        for (int16_t i = -255; i < 256; i++) {
            motor->setPWM(255 - abs(i));
            motor->setDir(i > 0);
            delay(2 * 2000 / 256);
        }
        motor->setPWM(0);
        delay(500);
    }
}