// Библиотека для робота МОШ (Jamaha)

#pragma once

#include "config.hpp"
#include "hardware/motor.hpp"
#include "hardware/linesensor.hpp"
#include "hardware/distancesensor.hpp"


// ПИНЫ
namespace pin {
    // MOTOРЫ
    enum MOTOR {
        // ЛЕВЫЙ
        ML_INVERT = false,
        ML_SPEED = 5,
        ML_DIR = 4,
        ML_ENC_A = 2, // ЖЕЛТЫЙ
        ML_ENC_B = 8, // ЗЕЛЁНЫЙ

        // ПРАВЫЙ
        MR_INVERT = true,
        MR_SPEED = 6,
        MR_DIR = 7,
        MR_ENC_A = 3, // ЖЕЛТЫЙ
        MR_ENC_B = 9, // ЗЕЛЁНЫЙ
    };

    enum LINES { L_SENSOR = A0, R_SENSOR = A1, }; // ДАТЧИКИ ЛИНИИ
    enum IR { IR_0 = A2, IR_1 = A3, }; // ИК-ДАТЧИК РАССТОЯНИЯ
    enum US { PIN_ECHO = 12, PIN_TRIG = 13, }; // УЗ-ДАТЧИК РАССТОЯНИЯ
}

extern hardware::LineSensor lineL;  // Левый датчик линии
extern hardware::LineSensor lineR;  //Правый датчик линии
extern hardware::IrSensorSharp ir0;  //Левый ИК-датчик расстояния
extern hardware::IrSensorSharp ir1;  // Правый ИК-датчик расстояния
extern hardware::UltraSonic us;  // Ультразвуковой датчик расстояния
extern hardware::MotorEncoder motorL;  // Левый мотор
extern hardware::MotorEncoder motorR;  // Правый мотор


// КОНФИГУРАЦИЯ

/**
 * @brief Указать ЛЕВЫЙ датчик расстояния
 * @param sensor ссылка на датчик
 */
void distSensorL(hardware::DistanceSensor& sensor);

/**
 * @brief Указать ПРАВЫЙ датчик расстояния
 * @param sensor ссылка на датчик
 */
void distSensorR(hardware::DistanceSensor& sensor);

/**
 * @brief Указать ПЕРЕДНИЙ датчик расстояния
 * @param sensor ссылка на датчик
 */
void distSensorF(hardware::DistanceSensor& sensor);


// ФУНКЦИИ ДВИЖЕНИЯ

/**
 * @brief Включить моторы с заданой скоростью на время
 * @param speed_L скорость левого мотора
 * @param speed_R скорость правого мотора
 * @param runtime время работы в мс
 * @param stop_at_exit Выключение ШИМ после завершения движения (= true)
 */
void setMotorsTime(int8_t speed_L, int8_t speed_R, uint32_t runtime, bool stop_at_exit = true);

/**
 * @brief Удерживать моторы в текущем положении на время
 * @param timeout время удержания
 */
void goHold(uint32_t timeout = PARAMS::HOLD_STOP_TIMEOUT);

/**
 * @brief Проехать 1000 тиков вперёд прямо
 * @param speed скорость движения
 */
void go_1000_ticks(uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Прямолинейное движение
 * @param distance_mm расстояние (отрицательное - едет назад)
 * @param speed скорость движения
 * @bug МК перезапускается при автономном движении со скоростью 50
 */
void goDirect(int32_t distance_mm, uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Прямолинейное движение до стены (Передний датчик)
 * @param wall_dist_cm расстояние до стены
 * @param speed скорость движения
 */
void goToWall(uint8_t wall_dist_cm, uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Прямолинейное движение до стены (любой датчик)
 * @param sensor ссылка на датчик
 * @param wall_dist_cm расстояние до стены
 * @param speed скорость движения
 */
void goToWall(hardware::DistanceSensor& sensor, uint8_t wall_dist_cm, uint8_t speed = PARAMS::DEFAULT_SPEED);

/**
 * @brief Поворот на заданый угол вокруг центра
 * @param a угол поворота ( <0 - поворот против часовой)
 * @param speed скорось поворота
 */
void turnAngle(int16_t a, uint8_t speed = PARAMS::DEFAULT_SPEED);


// ТЕСТЫ
namespace test {

    /**
     * @brief Измерить значения датчиков линий
     * @param ret_L возврат для левого
     * @param ret_R возврат для правого
     * @param timeout время измерения
     */
    void lines(uint16_t& ret_L, uint16_t& ret_R, uint32_t timeout = 1000);

    /// @brief тест функциональности моторов
    void motorsAccel();

    /// @brief тест функциональности моторов (ШИМ ВЕРСИЯ)
    void motorsPWM();
}
