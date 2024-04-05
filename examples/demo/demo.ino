// пины могут быть переопределены

#define MOTOR_L_INVERT false // Левый мотор инверсия направления
#define MOTOR_R_INVERT true // Правый мотор инверсия направления
#define MOTOR_L_ENC_A 2 // Левый мотор энкодер желтый
#define MOTOR_L_ENC_B 8 // Левый мотор энкодер зелёный
#define MOTOR_R_ENC_A 3 // Правый мотор энкодер желтый
#define MOTOR_R_ENC_B 9 // Правый мотор энкодер зелёный


#include <MoshLib.hpp>

void setup() {
    Serial.begin(9600);

    // Конфигурация пинов (Пинаут)

    lineL.pinout(A0) // пин ЛЕВОГО датчика линии - А0
        .config(57, 837); // значения на линии, на поле

    lineR.pinout(A1) // пин ЛЕВОГО датчика линии - А0
        .config(108, 981); // значения на линии, на поле

    ir0.pinout(A2); // пин ИК датчика 0 - А2
    ir1.pinout(A3); // пин ИК датчика 1 - А3

    us.pinout(12, 13); // задаём пины УЗ-датчика (ECHO, TRIG)


    // конфигурирование робота
    conf.mmInTicks1000(250) // 250 мм на 1000 тиков
        .track(177)         // 177 мм колея
        .speedPWM(128)      // ШИМ-скорость по умолчаюнию 128
        .speedTick(18)      // тик-скорость по умолчанию 18
        .distF(us)          // передний датчик - УЗ
        .distL(ir0)         // левый датчик - ИК0
        .distR(ir1)         // правый датчик - ИК1
    ;

    // проверки

    // goTick(1000); // проехать 1000 тиков


    // проверка моторов

    // motors.setDirPWM(255, 255); // ОБА ВПЕРЁД
    // motors.setDirPWM(255, 0); // ЛЕВОЕ ВПЕРЁД
    // motors.setDirPWM(-255, 0); // ЛЕВОЕ ВПЕРЁД
    // motors.setDirPWM(0, 255); // ПРАВОЕ ВПЕРЁД
    // motors.setDirPWM(0, -255); // ПРАВОЕ ВПЕРЁД

    // некоторые функции

    // goTime(1000);
    // goTime(1000, 12, -6);
    // goDist(1000);
    // goCross();
    // goCross(3); 
    // goHold(400)

    // lineTimeL(10000);
    // lineTimeR(10000);
    // lineTimeLR(10000);
    // lineTimeP(10000);
    // lineDistP(200)
    // lineCrossP();
    // lineCrossP(2);

    // turnAngle(90);
    // turnAngle(-90);
    // turnLineL();
    // turnLineL(2);
    // turnLineR();
    // turnLineR(2);

    // wallFront(30);
    // wallBack(30);
    // wallTimeL(30, 10000);
    // wallTimeR(30, 10000);
}

// макросы для лаконичности кода вывода
#define P(x) Serial.print(x)
#define PL(x) Serial.println(x)

void loop() {
    // ИК датчик 0
    // P(F("IR0 (cm):\t")); PL(ir0());

    // ИК датчик 1
    // P(F("IR1 (cm):\t")); PL(ir1());
    
    // УЗ-датчик
    // P(F("US  (cm):\t")); PL(us());

    // Линия L
    // P(F("LineL (%):\t")); P(lineL()); P(F("АЦП: \t")); P(lineL.readRaw()); P(F("on: ")); PL(lineL.on());

    // Линия R
    // P(F("LineR (%):\t")); P(lineR()); P(F("АЦП: \t")); P(lineR.readRaw()); P(F("on: ")); PL(lineR.on());


    // При ПРАВИЛЬНОМ подключении значения энкодера будут увеличиваться, 
    // если повернуть мотор так, чтобы он тянул робота вперёд
    // в обратном случае - уменьшаться

    // Энкодер L
    // P(F("Enc L")); PL(motorL.position);

    // Энкодер R
    // P(F("Enc R")); PL(motorR.position);
}
