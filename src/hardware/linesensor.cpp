#include "linesensor.hpp"
#include "robotconfig.hpp"


using namespace mosh::hardware;

LineSensor& LineSensor::pinout(uint8_t pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
    return *this;
}

LineSensor& LineSensor::config(uint16_t on_line, uint16_t on_field) {
    this->on_line = on_line;
    this->on_field = on_field;
    return *this;
}

int16_t LineSensor::readRaw() const { return analogRead(pin); }

int8_t LineSensor::read() const {
    int16_t ret = map(readRaw(), on_line, on_field, 100, 0);
    return (int8_t) constrain(ret, 0, 100);
}

int8_t LineSensor::operator () () const { return read(); }

bool LineSensor::on() const { return read() > conf.__line_gray_percent; }

LineSensor lineL, lineR;
