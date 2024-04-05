// подключения пинов могут быть переопределены

#define MOTOR_L_INVERT false // Левый мотор инверсия направления
#define MOTOR_R_INVERT true // Правый мотор инверсия направления
#define MOTOR_L_ENC_A 2 // Левый мотор энкодер желтый
#define MOTOR_L_ENC_B 8 // Левый мотор энкодер зелёный
#define MOTOR_R_ENC_A 3 // Правый мотор энкодер желтый
#define MOTOR_R_ENC_B 9 // Правый мотор энкодер зелёный


#include <MoshLib.hpp>

void setup() {
    lineL.pinout(A0).config(57, 837);
    lineR.pinout(A1).config(108, 981);
    ir0.pinout(A2);
    ir1.pinout(A3);
    us.pinout(12, 13);
}
void loop() {}
