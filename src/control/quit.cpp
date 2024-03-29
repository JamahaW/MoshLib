#include "quit.hpp"


using namespace mosh::control;

OnTimer::OnTimer(uint16_t duration) : END_TIME(millis() + duration) {}
bool OnTimer::tick() const { return millis() < END_TIME; }

IfDistanceSensorRead::IfDistanceSensorRead(hardware::DistanceSensor& used_sensor, const uint8_t target_distance, enum MODE condition) :
    sensor(used_sensor), DISTANCE(target_distance), mode((bool) condition) {}

bool IfDistanceSensorRead::tick() const { return (sensor.read() <= DISTANCE) ^ mode; }

DistanceMoved::DistanceMoved(int32_t target_mm) : TARGET(target_mm) { motors::reset(); }

bool DistanceMoved::tick() const { return abs(motorL.position - TARGET) * abs(motorR.position - TARGET) < 0; }
