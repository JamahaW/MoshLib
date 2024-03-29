#include "distancesensor.hpp"


using namespace mosh::hardware;

DistanceSensor::DistanceSensor(const uint16_t min_dist, const uint16_t max_dist, const uint8_t period)
    : MIN_DISTANCE(min_dist), MAX_DISTANCE(max_dist), READ_PERIOD(period) {}

uint8_t DistanceSensor::read() const {
    if (millis() >= next_read) {
        next_read = millis() + READ_PERIOD;
        value = update();
        value = constrain(value, MIN_DISTANCE, MAX_DISTANCE);
    }
    return value;
}

uint8_t DistanceSensor::operator () () const { return read(); }


// ИК ДАТЧИК РАССТОЯНИЯ

IrSensorSharp::IrSensorSharp(uint8_t pin) :
    DistanceSensor(PARAMS::IR_DIST_MIN, PARAMS::IR_DIST_MAX, PARAMS::IR_PERIOD), PIN(pin) {
    pinMode(PIN, INPUT);
}

uint8_t IrSensorSharp::update() const { return 32 * pow(analogRead(PIN) * (5.0 / 1024.0), -1.1); }


// УЗ ДАТЧИК РАССТОЯНИЯ

UltraSonic::UltraSonic(const uint8_t pin_echo, const uint8_t pin_trig) :
    DistanceSensor(PARAMS::US_DIST_MIN, PARAMS::US_DIST_MAX, PARAMS::US_PERIOD), PIN_ECHO(pin_echo), PIN_TRIG(pin_trig) {
    pinMode(PIN_ECHO, INPUT);
    pinMode(PIN_TRIG, OUTPUT);
}

uint8_t UltraSonic::update() const {
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(5);
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(15);
    digitalWrite(PIN_TRIG, LOW);
    uint32_t d = pulseIn(PIN_ECHO, HIGH);
    return d / (2 * 29.1);
}
