#pragma once

#include <Arduino.h>

#ifndef MOTOR_L_INVERT
// Левый мотор инверсия направления
#define MOTOR_L_INVERT false
#endif

// Левый мотор направление
#define MOTOR_L_SPEED 5

// Левый мотор скорость
#define MOTOR_L_DIR 4

#ifndef MOTOR_L_ENC_A
// Левый мотор энкодер желтый
#define MOTOR_L_ENC_A 2
#endif

#ifndef MOTOR_L_ENC_B
// Левый мотор энкодер зелёный
#define MOTOR_L_ENC_B 8
#endif

#ifndef MOTOR_R_INVERT
// Правый мотор инверсия направления
#define MOTOR_R_INVERT true
#endif

// Правый мотор скорость
#define MOTOR_R_SPEED 6

// Правый мотор направление
#define MOTOR_R_DIR 7

#ifndef MOTOR_R_ENC_A
// Правый мотор энкодер желтый
#define MOTOR_R_ENC_A 3
#endif

#ifndef MOTOR_R_ENC_B
// Правый мотор энкодер зелёный
#define MOTOR_R_ENC_B 9
#endif

#ifndef LINE_L
// Левый датчик линии
#define LINE_L A0
#endif

#ifndef LINE_R
// Правый датчик линии
#define LINE_R A1
#endif

#ifndef IR_0
// Инфракрасный датчик расстояния 0
#define IR_0 A2
#endif

#ifndef IR_1
// Инфракрасный датчик расстояния 1
#define IR_1 A3
#endif

#ifndef US_ECHO
// Ультразвуковой датчик расстояния приём
#define US_ECHO 12
#endif

#ifndef US_TRIG
// Ультразвуковой датчик расстояния излучатель
#define US_TRIG 13
#endif


// TODO вынести макроконстанты
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

// .. где мой constexpr, Ардуино!!? где С++11???
// Переводной макрос из ММ пути в тики энкодера
#define MM2TICKS(mm) ( (int32_t)(mm) * 1000L / (int32_t)(PARAMS::MM_IN_1000_TICKS) ) 

// // TODO вырезать!!
// // виды регуляторов движения по линии 
// enum LINE_REGULATORS { RELAY_L, RELAY_R, RELAY_LR, PROP };