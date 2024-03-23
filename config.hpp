#pragma once

// КОНФИГУРАЦИЯ
namespace cfg
{
    // МОТОРЫ
    enum MOTOR
    {
        MM_IN_1000_TICKS = 250, // В 1000 ТИКАХ
        DEFAULT_SPEED = 8,      // скорость движения по умолчанию
        TRACK = 178,            // КОЛЕЯ

        // ЛЕВЫЙ
        ML_INVERT = false,
        ML_SPEED = 5,
        ML_DIR = 4,
        ML_ENC_A = 2, // ЖЕЛТЫЙ
        ML_ENC_B = 8, // ЗЕЛЁНЫЙ

        // ПРАВЫЙ
        MR_INVERT = true,
        MR_SPEED = 6,
        MR_DIR = 7,
        MR_ENC_A = 3, // ЖЕЛТЫЙ
        MR_ENC_B = 9, // ЗЕЛЁНЫЙ
    };

    // ДАТЧИК ЛИНИИ
    enum LINE
    {
        L_SENSOR = A0,
        L_LINE = 57,
        L_FIELD = 837,

        R_SENSOR = A1,
        R_LINE = 108,
        R_FIELD = 981,

        GRAY_PERCENT = 50,
    };

    // ИК-Датчик расстояния
    enum IR
    {
        L_PIN = A2,
        R_PIN = A3,

        I_MIN_DIST = 90,
        I_MAX_DIST = 500,
        I_PERIOD = 15,
    };
    
    // УЗ-Датчик расстояния
    enum US
    {
        PIN_ECHO = 12,
        PIN_TRIG = 13,

        U_MIN_DIST = 25,
        U_MAX_DIST = 1000,
        U_PERIOD = 100,
    };
}
