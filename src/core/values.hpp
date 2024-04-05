#pragma once

enum ENV_PARAMS {
    KP_SPEED = 8, // KP смещение (скорость)
    SYNC_K = 5, // коэф. синх. движения 
    MAX_DELTA_TICKS = 32, // максимальная d_tick при повороте
    SPIN_PERIOD_MS = 32, // период смены целевого положения мотора по d_tick

    IR_DIST_MIN = 9,
    IR_DIST_MAX = 50,
    IR_PERIOD = 15,

    US_DIST_MIN = 2,
    US_DIST_MAX = 100,
    US_PERIOD = 200,
};


enum CONF_DEFAULT {
    LINE_GRAY_PERCENT = 50,
    LINE_ON_LINE = 82,
    LINE_ON_FIELD = 909,

    HOLD_DURATION = 160, // время остановки удержанием позиции
    ALIGN_DURATION = 400, // время для выравнивания по линии

    TRACK_SIZE_MM = 177,
    MM_IN_1000_TICKS = 250,

    SPEED_TICK = 20,
    SPEED_PWM = 144,
};
