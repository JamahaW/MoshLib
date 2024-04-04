#include "distancesensor.hpp"


using namespace mosh::hardware;

DistanceSensor::DistanceSensor(const uint16_t min_dist, const uint16_t max_dist, const uint8_t period)
    : DIST_FLOOR(min_dist), DIST_CEIL(max_dist), PERIOD(period) {}

uint8_t DistanceSensor::read() const {
    if (millis() >= timer) {
        timer = millis() + PERIOD;
        value = update();
        value = constrain(value, DIST_FLOOR, DIST_CEIL);
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
    DistanceSensor(PARAMS::US_DIST_MIN, PARAMS::US_DIST_MAX, PARAMS::US_PERIOD), ECHO(pin_echo), TRIG(pin_trig) {
    pinMode(ECHO, INPUT);
    pinMode(TRIG, OUTPUT);
}

uint8_t UltraSonic::update() const {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(15);
    digitalWrite(TRIG, LOW);
    uint32_t d = pulseIn(ECHO, HIGH);
    return d / (2 * 29.1);
}
