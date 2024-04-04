#pragma once

#include <Arduino.h>

// TODO перенести большую часть конфигураций в методы тех или 

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

#ifdef LINE_L
// #define LINE_L A0 // Левый датчик линии
#warning LINE_L - этот макрос больше не поддерживается, используйте метод lineL.setPin(uint8_t pin)
#endif

#ifdef LINE_R
// #define LINE_R A1 // Правый датчик линии
#warning LINE_R - этот макрос больше не поддерживается, используйте метод lineR.setPin(uint8_t pin)
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
#define DEFAULT_SPEED_TICK 18 // Скорость в тиках по умолчанию
#define DEFAULT_SPEED_TICK 18 // Скорость в тиках по умолчанию
#endif

#ifndef DEFAULT_SPEED_PWM
#define DEFAULT_SPEED_PWM 144 // Скорость в ШИМ по умолчанию
#endif

#ifndef GRAY_PERCENT
#define GRAY_PERCENT 50 // Уровень чувствительности датчика линии
#endif

#ifdef R_LINE
// #define R_LINE 108 // значеие ПРАВОГО датчика на ЛИНИИ 
#warning R_LINE - этот макрос больше не поддерживается, используйте метод lineR.config(uint16_t on_line, uint16_t on_field)
#warning В ином случае в качестве начальных значений будет PARAMS::LINE_DEFAULT_ON_LINE
#endif

#ifdef R_FIELD
// #define R_FIELD 981// значеие ПРАВОГО датчика на ПОЛЕ
#warning R_FIELD - этот макрос больше не поддерживается, используйте метод lineR.config(uint16_t on_line, uint16_t on_field)
#warning В ином случае в качестве начальных значений будет PARAMS::LINE_DEFAULT_ON_FIELD
#endif

#ifdef L_LINE
// #define L_LINE 57 // значеие ЛЕВОГО датчика на ЛИНИИ 
#warning L_LINE - этот макрос больше не поддерживается, используйте метод lineL.config(uint16_t on_line, uint16_t on_field)
#warning В ином случае в качестве начальных значений будет PARAMS::LINE_DEFAULT_ON_LINE
#endif

#ifdef L_FIELD
// #define L_FIELD 837 // значеие ЛЕВОГО датчика на ПОЛЕ 
#warning L_FIELD - этот макрос больше не поддерживается, используйте метод lineL.config(uint16_t on_line, uint16_t on_field)
#warning В ином случае в качестве начальных значений будет PARAMS::LINE_DEFAULT_ON_FIELD
#endif


// КОНФИГУРАЦИЯ
enum PARAMS {
    HOLD_STOP_TIMEOUT = 160, // время остановки удержанием позиции
    ALIGN_DURATION = 400, // время для выравнивания по линии

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

enum DEFAULT_CONF {
    LINE_ON_LINE = 82,
    LINE_ON_FIELD = 909
};

#define SIGN(x) (((x) > 0) ? 1 : -1) // ЗНАК переменной