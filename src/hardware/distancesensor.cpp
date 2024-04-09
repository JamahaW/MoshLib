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

/// @brief ИК-датчик расстояния: минимальное расстояние
#define INFORED_MIN_CM 9

/// @brief ИК-датчик расстояния: максимальное расстояние
#define INFORED_MAX_CM 50

/// @brief ИК-датчик расстояния: период обновления
#define INFORED_PERIOD_MS 15


IrSensorSharp::IrSensorSharp() : DistanceSensor(INFORED_MIN_CM, INFORED_MAX_CM, INFORED_PERIOD_MS) {}

uint8_t IrSensorSharp::update() const { return 32 * pow(analogRead(pin) * (5.0 / 1024.0), -1.1); }

void IrSensorSharp::pinout(uint8_t pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
}


// УЗ ДАТЧИК РАССТОЯНИЯ

/// @brief УЗ-датчик расстояния: минимальное расстояние
#define ULTRA_SONIC_MIN_CM 2

/// @brief УЗ-датчик расстояния: максимальное расстояние
#define ULTRA_SONIC_MAX_CM 100

/// @brief УЗ-датчик расстояния: период обновления
#define ULTRA_SONIC_PERIOD_MS 200


UltraSonic::UltraSonic() : DistanceSensor(ULTRA_SONIC_MIN_CM, ULTRA_SONIC_MAX_CM, ULTRA_SONIC_PERIOD_MS) {}

uint8_t UltraSonic::update() const {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(15);
    digitalWrite(TRIG, LOW);
    uint32_t d = pulseIn(ECHO, HIGH);
    return d / (2 * 29.1);
}

void UltraSonic::pinout(uint8_t pin_echo, uint8_t pin_trig) {
    ECHO = pin_echo;
    TRIG = pin_trig;
    pinMode(ECHO, INPUT);
    pinMode(TRIG, OUTPUT);
}

NoDistanceSensor __no_sensor;
IrSensorSharp ir0, ir1;
UltraSonic us;
