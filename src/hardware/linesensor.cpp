#include "linesensor.hpp"


using namespace mosh::hardware;

LineSensor::LineSensor(const uint8_t pin, const uint16_t on_line, const uint16_t on_field)
    : PIN(pin), ON_LINE(on_line), ON_FIELD(on_field) {
    pinMode(PIN, INPUT);
}

int16_t LineSensor::readRaw() const { return analogRead(PIN); }

int8_t LineSensor::read() const {
    int16_t ret = map(readRaw(), ON_LINE, ON_FIELD, 100, 0);
    return (int8_t) constrain(ret, 0, 100);
}

int8_t LineSensor::operator () () const { return read(); }

bool LineSensor::on() { return read() > GRAY_PERCENT; }