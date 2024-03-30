#pragma once

#include <Arduino.h>


#ifndef MOTOR_L_INVERT
#define MOTOR_L_INVERT false // Левый мотор инверсия направления
#endif

#define MOTOR_L_SPEED 5 // Левый мотор направление

#define MOTOR_L_DIR 4 // Левый мотор скорость

#ifndef MOTOR_L_ENC_A
#define MOTOR_L_ENC_A 2 // Левый мотор энкодер желтый
#endif

#ifndef MOTOR_L_ENC_B
#define MOTOR_L_ENC_B 8 // Левый мотор энкодер зелёный
#endif

#ifndef MOTOR_R_INVERT
#define MOTOR_R_INVERT true // Правый мотор инверсия направления
#endif

#define MOTOR_R_SPEED 6 // Правый мотор скорость

#define MOTOR_R_DIR 7 // Правый мотор направление

#ifndef MOTOR_R_ENC_A
#define MOTOR_R_ENC_A 3 // Правый мотор энкодер желтый
#endif

#ifndef MOTOR_R_ENC_B
#define MOTOR_R_ENC_B 9 // Правый мотор энкодер зелёный
#endif

#ifndef LINE_L
#define LINE_L A0 // Левый датчик линии
#endif

#ifndef LINE_R
#define LINE_R A1 // Правый датчик линии
#endif

#ifndef IR_0
#define IR_0 A2 // Инфракрасный датчик расстояния 0
#endif

#ifndef IR_1
#define IR_1 A3 // Инфракрасный датчик расстояния 1
#endif

#ifndef US_ECHO
#define US_ECHO 12 // Ультразвуковой датчик расстояния приём
#endif

#ifndef US_TRIG
#define US_TRIG 13 // Ультразвуковой датчик расстояния излучатель
#endif

#ifndef TRACK_SIZE_MM
#define TRACK_SIZE_MM 177 // КОЛЕЯ
#endif

#ifndef MM_IN_1000_TICKS
#define MM_IN_1000_TICKS 250 // В 1000 ТИКАХ
#endif

#define MM2TICKS(mm) ( (int32_t)(mm) * 1000L / (int32_t)(MM_IN_1000_TICKS) )  // Переводной макрос из ММ пути в тики энкодера

#ifndef DEFAULT_SPEED_TICK
#define DEFAULT_SPEED_TICK 9 // Скорость в тиках по умолчанию
#endif

#ifndef DEFAULT_SPEED_PWM
#define DEFAULT_SPEED_PWM 144 // Скорость в ШИМ по умолчанию
#endif

#ifndef GRAY_PERCENT
#define GRAY_PERCENT 50 // Уровень чувствительности датчика линии
#endif

#ifndef R_LINE
#define R_LINE 108 // значеие ПРАВОГО датчика на ЛИНИИ 
#endif

#ifndef R_FIELD
#define R_FIELD 981// значеие ПРАВОГО датчика на ПОЛЕ 
#endif

#ifndef L_LINE
#define L_LINE 57 // значеие ЛЕВОГО датчика на ЛИНИИ 
#endif

#ifndef L_FIELD
#define L_FIELD 837 // значеие ЛЕВОГО датчика на ПОЛЕ 
#endif


// TODO вынести макроконстанты
// КОНФИГУРАЦИЯ
enum PARAMS {
    HOLD_STOP_TIMEOUT = 400, // время остановки удержанием позиции

    KP_SPEED = 8, // KP смещение (скорость)
    KP_1_FOLLOW = 4, // 1/KP стремление в точку
    SYNC_K = 3, // коэф. синх. движения 
    MAX_DELTA_TICKS = 32, // максимальная d_tick при повороте
    SPIN_PERIOD_MS = 32, // период смены целевого положения мотора по d_tick

    IR_DIST_MIN = 9,
    IR_DIST_MAX = 50,
    IR_PERIOD = 15,

    US_DIST_MIN = 2,
    US_DIST_MAX = 100,
    US_PERIOD = 200,
};

#define SIGN(x) (((x) > 0) ? 1 : -1) // ЗНАК переменной