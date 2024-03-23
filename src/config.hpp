#pragma once

// КОНФИГУРАЦИЯ
enum PARAMS
{
    TRACK = 178, // КОЛЕЯ
    MM_IN_1000_TICKS = 250, // В 1000 ТИКАХ

    HOLD_STOP_TIMEOUT = 300, // время остановки удержанием позиции

    KP_SPEED = 8, // KP смещение (скорость)
    KP_1_FOLLOW = 4, // 1/KP стремление в точку

    MAX_DELTA_TICKS = 50, // максимальная d_tick при повороте
    SPIN_PERIOD_MS = 32, // период смены целевого положения мотора по d_tick
    DEFAULT_SPEED = ((int)MAX_DELTA_TICKS / 6), // скорость движения по умолчанию

    GRAY_PERCENT = 50,

    IR_MIN_DIST = 9,
    IR_MAX_DIST = 50,
    IR_PERIOD = 15,

    US_MIN_DIST = 2,
    US_MAX_DIST = 100,
    US_PERIOD = 100,

    R_LINE = 108,
    R_FIELD = 981,

    L_LINE = 57,
    L_FIELD = 837,

};
