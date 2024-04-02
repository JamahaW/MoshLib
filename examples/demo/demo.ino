// ДЕМО

#include <Arduino.h>

// подключения пинов могут быть переопределены
// ВАЖНО это сделать перед подключением библиотеки

#define MOTOR_L_INVERT false // Левый мотор инверсия направления
#define MOTOR_R_INVERT true // Правый мотор инверсия направления

#define MOTOR_L_ENC_A 2 // Левый мотор энкодер желтый
#define MOTOR_L_ENC_B 8 // Левый мотор энкодер зелёный

#define MOTOR_R_ENC_A 3 // Правый мотор энкодер желтый
#define MOTOR_R_ENC_B 9 // Правый мотор энкодер зелёный

#define LINE_L A0 // Левый датчик линии
#define LINE_R A1 // Правый датчик линии

#define IR_0 A2 // Инфракрасный датчик расстояния 0
#define IR_1 A3 // Инфракрасный датчик расстояния 1

#define US_ECHO 12 // Ультразвуковой датчик расстояния приём
#define US_TRIG 13 // Ультразвуковой датчик расстояния излучатель

#include <MoshLib.hpp>


void setup() {}

void loop() {}