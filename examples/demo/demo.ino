#include <MoshLib.hpp>

void setup() {
    Serial.begin(9600);

    // Конфигурация пинов (Пинаут)

    lineL.pinout(A0) // пин ЛЕВОГО датчика линии - А0
        .config(57, 837); // значения на линии, на поле

    lineR.pinout(A1) // пин ПРАВОГО датчика линии - А1
        .config(108, 981); // значения на линии, на поле

    ir0.pinout(A2); // пин ИК датчика 0 - А2
    ir1.pinout(A3); // пин ИК датчика 1 - А3

    us.pinout(12, 13); // задаём пины УЗ-датчика (ECHO, TRIG)

    // motorL motorR motors

    motorL
        .encoder(2, 8, false) // Энкодер на пинах 2, 8, не инвертирован
        .pinout(5, 4, false); // Мотор на пинах 5, 4, не инвертирован

    motorR
        .encoder(3, 9, true) // Энкодер на пинах 3, 9, инвертирован
        .pinout(6, 7, true); // Мотор на пинах 6, 7, инвертирован

    // конфигурирование робота
    conf.mmInTicks1000(250) // 250 мм на 1000 тиков
        .track(177)         // 177 мм колея
        .speedPWM(128)      // ШИМ-скорость по умолчаюнию 128
        .speedTick(18)      // тик-скорость по умолчанию 18
        .distF(us)          // передний датчик - УЗ
        .distL(ir0)         // левый датчик - ИК0
        .distR(ir1)         // правый датчик - ИК1
        .holdTime(500)      // длительность удержания позиции
        .alignTime(1000)    // длительность вырвавнивания
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

    // goTime(1000); // вперёд прямо 1000мс с базовой скоростью
    // goTime(1000, 16); // вперёд прямо 1000мс с скоростью 16тик
    // goTime(1000, 12, -6); // 1000 мс левое колесо крутится скорость 12тик, правое -6тик
    // goDist(1000); прямолинейное синхронное движение на расстояние
    // goDist(-100); назад на 100мм
    // goCross(); прямолинейное синхронное движение до перекрёстка
    // goCross(3); до 3 перекрёстка
    // goHold(400) удерживание моторов в текущем положении в течении 400мс

    // алгоритмы движения по линии
    // lineTimeL(10000); // 10000 мс по левому датчику Реле
    // lineTimeR(10000); // по правому датчику Реле
    // lineTimeLR(10000); // Два датчика, релейный регулятор
    // lineTimeP(10000); // пропорциональный регулятор

    // lineDistP(200) // 200 мм по линии с пропорциональный регулятор
    // lineCrossP(); // до первого перекрёстка
    // lineCrossP(4); // до 4 перекрёстка

    // turnAngle(90); // синхронный поворот вокруг центра на 90 градусов по часовой вокруг центра
    // turnAngle(-90); // против часовой
    // turnLineL(); // до линии слева
    // turnLineR(); // справа
    // turnLineL(2); // остановится на 2-м луче перекрёстка
    // turnLineR(2); // вправо

    // wallFront(30); // прямолинейное синхронное движение пока ПЕРЕДНИЙ датчик линии не зафиксирует <30 см
    // wallBack(30); // назад, пока не будет >30см
    // wallTimeL(30, 10000); // вдоль стены по ЛЕВОМУ датчику
    // wallTimeR(30, 10000); // по правому
}

// макросы для лаконичности кода вывода
#define P(x) Serial.print(x)
#define PF(s) P(F(s))
#define PL(x) Serial.println(x)

void loop() {
    // ИК датчик 0
    // PF("IR0 (cm):\t"); PL(ir0());

    // ИК датчик 1
    // PF("IR1 (cm):\t"); PL(ir1());

    // УЗ-датчик
    // PF("US  (cm):\t"); PL(us());

    // Линия L
    // PF("LineL (%):\t"); P(lineL()); PF("АЦП: \t"); P(lineL.readRaw()); PF("on: "); PL(lineL.on());

    // Линия R
    // PF("LineR (%):\t"); P(lineR()); PF("АЦП: \t"); P(lineR.readRaw()); PF("on: "); PL(lineR.on());


    // При ПРАВИЛЬНОМ подключении значения энкодера будут увеличиваться, 
    // если повернуть мотор так, чтобы он тянул робота вперёд
    // в обратном случае - уменьшаться

    // Энкодер L
    // PF("Enc L"); PL(motorL.position);

    // Энкодер R
    // PF("Enc R"); PL(motorR.position);
}
