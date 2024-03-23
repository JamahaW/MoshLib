#pragma once

#include <Arduino.h>
#include "config.hpp"


namespace sys {

    /**
     * @brief Интерфейс датчика растояния
     */
    class I_DistanceSensor {

    private:

        /// @brief Нижняя граница отсечения расстояния
        const uint16_t MIN_DISTANCE;

        /// @brief Верхняя граница отсеченя расстояния
        const uint16_t MAX_DISTANCE;

        /// @brief Период чтения
        const uint8_t READ_PERIOD;

        /// @brief время следующего опроса датчика
        mutable uint32_t next_read = 0;

        /// @brief предыдущее значение датчика
        mutable uint8_t value = 0;

    protected:

        /**
         * @brief Опросить датчик
         * @return значение расстояние
         */
        virtual const uint8_t process() const = 0;

    public:

        I_DistanceSensor(const uint16_t min_dist, const uint16_t max_dist, const uint8_t period)
            : MIN_DISTANCE(min_dist), MAX_DISTANCE(max_dist), READ_PERIOD(period) {}

        /**
         * @brief Получить значение с датчика
         * @return расстояние датчика (мм)
         */
        uint8_t read() const {
            if (millis() >= next_read) {
                next_read = millis() + READ_PERIOD;
                value = process();
                value = constrain(value, MIN_DISTANCE, MAX_DISTANCE);
            }
            return value;
        }

        uint8_t operator () () const { return read(); }
    };

    /**
     * @brief Инфокрасный датчик Sharp
     */
    class IrSensorSharp : public I_DistanceSensor {

    private:

        const uint8_t PIN;

    protected:

        const uint8_t process() const override { return 32 * pow(analogRead(PIN) * (5.0 / 1024.0), -1.1); }

    public:

        /**
         * @brief ИК-датчик расстояния
         * @param pin Аналоговый пин (A0 - A7)
         */
        IrSensorSharp(const uint8_t pin) : I_DistanceSensor(PARAMS::IR_MIN_DIST, PARAMS::IR_MAX_DIST, PARAMS::IR_PERIOD), PIN(pin) {
            pinMode(PIN, INPUT);
        }
    };

    /**
     * @brief УльтраЗвуковой датчик HC SR04
     */
    class UltraSonic : public I_DistanceSensor {

    private:

        const uint8_t PIN_ECHO, PIN_TRIG;

    protected:

        const uint8_t process() const override {
            digitalWrite(PIN_TRIG, LOW);
            delayMicroseconds(5);
            digitalWrite(PIN_TRIG, HIGH);
            delayMicroseconds(15);
            digitalWrite(PIN_TRIG, LOW);
            uint32_t d = pulseIn(PIN_ECHO, HIGH);
            return d / (2 * 29.1);
        }

    public:

        /**
         * @brief Ультразвуковой датчик расстояния
         * @param pin_echo Пин приёма
         * @param pin_trig Пин Триггера
         */
        UltraSonic(const uint8_t pin_echo, const uint8_t pin_trig) :
            I_DistanceSensor(PARAMS::US_MIN_DIST, PARAMS::US_MAX_DIST, PARAMS::US_PERIOD),
            PIN_ECHO(pin_echo),
            PIN_TRIG(pin_trig)
        {
            pinMode(PIN_ECHO, INPUT);
            pinMode(PIN_TRIG, OUTPUT);
        }

    };

}