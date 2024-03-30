#include "quit.hpp"


using namespace mosh::control;

OnTimer::OnTimer(uint16_t duration) : END_TIME(millis() + duration) {}
bool OnTimer::tick() const { return millis() < END_TIME; }

DistanceRead::DistanceRead(hardware::DistanceSensor& used_sensor, const uint8_t target_distance, enum MODE condition) :
    sensor(used_sensor), DISTANCE(target_distance), mode((bool) condition) {}

bool DistanceRead::tick() const { return (sensor.read() <= DISTANCE) ^ mode; }

DistanceMoved::DistanceMoved(int32_t dist_l, int32_t dist_r, bool mm_mode) :
    TARGET_L(mm_mode ? MM2TICKS(dist_l) : dist_l), TARGET_R(mm_mode ? MM2TICKS(dist_r) : dist_r) {
    motors::reset();
}

DistanceMoved::DistanceMoved(int32_t distance) : DistanceMoved(distance, distance) {}

bool DistanceMoved::tick() const { return (abs(motorL.position - TARGET_L) > 0) && (abs(motorR.position - TARGET_R) > 0); }
