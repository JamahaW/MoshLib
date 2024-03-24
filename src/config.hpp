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

    IR_DIST_MIN = 9,
    IR_DIST_MAX = 50,
    IR_PERIOD = 15,

    US_DIST_MIN = 2,
    US_DIST_MAX = 100,
    US_PERIOD = 100,

    GRAY_PERCENT = 50,
    R_LINE = 108,
    R_FIELD = 981,
    L_LINE = 57,
    L_FIELD = 837,
};
