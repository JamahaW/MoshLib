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

DistanceMoved::DistanceMoved(int32_t distance) : DistanceMoved(distance, distance) {motors::reset();}

// TODO расчёт среднего расстояния
bool DistanceMoved::tick() const { return (abs(motorL.position - TARGET_L) > 0) && (abs(motorR.position - TARGET_R) > 0); }

static bool __on_line_left() { return lineL.on(); }
static bool __on_line_right() { return lineR.on(); }
static bool __on_line_both() { return lineL.on() && lineR.on(); }

LineFound::LineFound(MODE mode, uint8_t crosses, bool exit_at) :
    EXIT(exit_at), trigger((mode == LINE_LEFT) ? __on_line_left : (mode == LINE_RIGHT) ? __on_line_right : __on_line_both), CROSSES(crosses) {}

bool LineFound::tick() const {
    bool state = trigger() ^ EXIT;

    if (state) {
        if (!found) {
            found = true;
            passed++;
        }
    }
    else {
        found = false;
    }

    return passed < CROSSES;
}
