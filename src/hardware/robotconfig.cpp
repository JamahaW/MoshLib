#include "robotconfig.hpp"
#include "hardware/distancesensor.hpp"


using namespace mosh::hardware;

RobotConfig::RobotConfig() { stock(); }

int32_t RobotConfig::mm2ticks(int32_t mm) { return mm * 1000L / __mm_in_1000_ticks; }

RobotConfig& RobotConfig::stock() {
    // значения датчиков по умолчанию
    dist_left = &__no_sensor;
    dist_right = &__no_sensor;
    dist_front = &__no_sensor;

    __line_gray_percent = 30;

    // скорости
    __default_speed_tick = 16;
    __default_speed_pwm = 144;

    // кинематика
    __track_size = 177;
    __mm_in_1000_ticks = 250;

    // тайминги
    __align_duration = 400;
    __hold_duration = 150;

    return *this; // вернуть себя же
}

RobotConfig& RobotConfig::distL(DistanceSensor& sensor) { dist_left = &sensor; return *this; }

RobotConfig& RobotConfig::distR(DistanceSensor& sensor) { dist_right = &sensor; return *this; }

RobotConfig& RobotConfig::distF(DistanceSensor& sensor) { dist_front = &sensor; return *this; }

RobotConfig& RobotConfig::track(uint8_t track_size) { this->__track_size = track_size; return *this; }

RobotConfig& RobotConfig::speedTick(uint8_t ticks) { __default_speed_tick = ticks; return *this; }

RobotConfig& RobotConfig::speedPWM(uint8_t pwm) { __default_speed_pwm = pwm; return *this; }

RobotConfig& RobotConfig::mmInTicks1000(uint16_t mm_in_1000_ticks) { this->__mm_in_1000_ticks = mm_in_1000_ticks; return *this; }

RobotConfig& RobotConfig::alignTime(uint16_t duration) {
    __align_duration = duration;
    return *this;
}

RobotConfig& RobotConfig::holdTime(uint16_t duration) {
    __hold_duration = duration;
    return *this;
}

RobotConfig& RobotConfig::lineGray(uint8_t percent) {
    __line_gray_percent = percent;
    return *this;
}

RobotConfig conf;
