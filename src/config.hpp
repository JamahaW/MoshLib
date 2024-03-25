#pragma once

#include <Arduino.h>

// ПИНЫ
namespace pin {
// MOTOРЫ
enum MOTOR {
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
// ДАТЧИКИ ЛИНИИ
enum LINES {
    L_SENSOR = A0,
    R_SENSOR = A1,
};
// ИК-ДАТЧИК РАССТОЯНИЯ
enum IR {
    IR_0 = A2,
    IR_1 = A3,
};
// УЗ-ДАТЧИК РАССТОЯНИЯ
enum US {
    ECHO = 12,
    TRIG = 13,
};
}

// КОНФИГУРАЦИЯ
enum PARAMS
{
    TRACK = 178, // КОЛЕЯ
    MM_IN_1000_TICKS = 250, // В 1000 ТИКАХ

    HOLD_STOP_TIMEOUT = 300, // время остановки удержанием позиции

    KP_SPEED = 8, // KP смещение (скорость)
    KP_1_FOLLOW = 4, // 1/KP стремление в точку
    MAX_DELTA_TICKS = 32, // максимальная d_tick при повороте
    SPIN_PERIOD_MS = 32, // период смены целевого положения мотора по d_tick
    DEFAULT_SPEED = ((int) MAX_DELTA_TICKS / 4), // скорость движения по умолчанию

    IR_DIST_MIN = 9,
    IR_DIST_MAX = 50,
    IR_PERIOD = 15,

    US_DIST_MIN = 2,
    US_DIST_MAX = 100,
    US_PERIOD = 200,

    GRAY_PERCENT = 50,
    R_LINE = 108,
    R_FIELD = 981,
    L_LINE = 57,
    L_FIELD = 837,
};

// виды регуляторов движения по линии
enum LINE_REGULATORS {
    RELAY_L,
    RELAY_R,
    RELAY_LR,
    PROP,
};
