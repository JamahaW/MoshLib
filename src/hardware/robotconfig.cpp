#include "robotconfig.hpp"
#include "hardware/distancesensor.hpp"


using namespace mosh::hardware;

int32_t RobotConfig::mm2ticks(int32_t mm) { return mm * 1000L / mm_in_1000_ticks; }

void RobotConfig::distL(DistanceSensor& sensor) { dist_left = &sensor; }

void RobotConfig::distR(DistanceSensor& sensor) { dist_right = &sensor; }

void RobotConfig::distF(DistanceSensor& sensor) { dist_front = &sensor; }

void RobotConfig::track(uint8_t trasksize_mm) { track_size = trasksize_mm; }

void RobotConfig::speedTick(uint8_t ticks) { default_speed_tick = ticks; }

void RobotConfig::speedPWM(uint8_t pwm) { default_speed_pwm = pwm; }

void RobotConfig::mmInTicks1000(uint16_t mm_in_1000_ticks) { this->mm_in_1000_ticks = mm_in_1000_ticks; }

RobotConfig conf;
