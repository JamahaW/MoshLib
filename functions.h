
/* список функций
rinit() - инициализация элементов робота, вставляется в setup
end() - завершает выполнение программы
stop(time) - торможение моторов в течение времени

rideDist(dist, speed) - проезд вперед/назад на определенную дистанцию, отрицательная дистанция - робот будет ехать назад
rideHalf() - проезд вперед на половину корпуса

turnToLine(dir, speed) - поворот влево/вправо до линии(на месте), dir принимает значения LEFT или RIGHT
turnAngle(angle, speed) - поворот влево/вправо на угол(на месте), положительный угол - поворот по часовой, отрицательный - против часовой


rideLine(stop_type, argument, movement_type, sensor) - езда по линии
возможные алгоритмы движения: pd - регулятор, зиг-заг по левому, правому или по обоим датчикам
в программе используется как PD_REG, ZIG_ZAG_LEFT, ZIG_ZAG_RIGHT, ZIG_ZAG_BOTH

возможные варианты остановки: по времени, по расстоянию, пока не увидит стенку, до n-ного перекрестка, без остановки
в программе используется как ON_TIME, BY_RIDE_DISTANCE, ON_WALL_DISTANCE, ON_CROSS, NONE

в качестве argument вводится аргумент для остановки, например количество перекрестков, расстояние, время езды и т.д.
в sensor может вводится только US или IR, что означает тип датчика расстояния который будет использоваться при езде до обнаружения стенки

обязательные только первые 2 параметра, а именно условие остановки и аргумент для этого условия, также можно по желанию менять алгоритм движения по линии и тип дачитка расстояния
в зависимости от того, с какого датчика получаются данные

rideWall(goal_dist, reversed, stop_type, argument, sensor, movement_type) - езда по стенке

goal_dist - расстояние от робота до стенки на котором он будет пытаться держаться
reversed - если датчик расстояния расположен слева то true, если справа то false
stop_type - аналогично stop_type из rideLine(), однако отсутвует остановка по перекресткам и до появления стенки, вместо этого
используется BY_END_WALL(ехать пока стенка не закончится), ON_LINE(робот будет ехать пока не увидит линию одним из датчиков)
sensor - тип датчика расстояния аналогично sensor из rideLine
movement_type - есть pd регулятор и зиг-заг. Возможные значения: PD_REG, ZIG_ZAG
*/

//подключение компонентов

//                                ______________________________________________
//                               -| 0                                       A5 |-
//                               -| 1                                       A4 |-
//                 R_MOTOR_ENC_A -| 2                                       A3 |-
//                 L_MOTOR_ENC_A -| 3                                       A2 |- IR_DIST
//                   L_MOTOR_DIR -| 4-H2                                    A1 |- R_LINE
//                 L_MOTOR_SPEED -| 5-E2                                    A0 |- L_LINE
//                 R_MOTOR_SPEED -| 6-E1                                       |
//                   R_MOTOR_DIR -| 7-H1                                   VIN |
//                 R_MOTOR_ENC_B -| 8                                      GND |
//                 L_MOTOR_END_B -| 9                                      GND |
//                  US_DIST_ECHO -| 10                                      5V |
//                  US_DIST_TRIG -| 11                                    3.3V |
//                         SERVO -| 12                                   RESET |
//                               -| 13                                   IOREF |
//                                |                                            |-
//                               -| GND                                    ОБЩ |--|
//                               -| VCC                                    ПИТ |--|
//                               -| SDA                                        |
//                               -| SCL                                        |
//                                |                                            |
//                                |____                                        |
//                                | O | RESET                                  |
//                                |___|___________M1+_M1-___M2+_M2-____ + _ - _|
//                                                  |  |    |   |       |   |

//свободные пины можно использовать по своему усмотрению, однако данной библиотекой это учитываться не будет

#pragma once
#include "ride.h"
#include "display.h"

#define Reverse_servo false

void rinit() {
  init_motors();
  pinMode(PIN_DIST_US_ECHO, INPUT);
  pinMode(PIN_DIST_US_TRIG, OUTPUT);
  pinMode(PIN_DIST_IR, INPUT);

  Serial.begin(9600);
  delay(1000);
  servo.attach(PIN_SERVO);
}
void end() {
  stopMotors();
  exit(0);
}
void stop(uint16_t timer = 500) {
  stopMotors(timer);
}
void rideDist(int32_t distance_mm, int speed = 50) {
  leftMotor.resetTicks();
  rightMotor.resetTicks();
  int32_t final_ticks;

  if (distance_mm > 0) {
    final_ticks = distance_mm * MM_TO_TICK;
    while (min(leftMotor.ticks, rightMotor.ticks) < final_ticks) {
      int16_t l = leftMotor.ticks;
      int16_t r = rightMotor.ticks;
      int16_t err = (l - r) * ENC_KOEF;
      setMotors(speed - err, speed + err);
    }
  } else if (distance_mm < 0) {
    final_ticks = distance_mm * MM_TO_TICK;
    while (max(leftMotor.ticks, rightMotor.ticks) > final_ticks) {
      int16_t l = leftMotor.ticks;
      int16_t r = rightMotor.ticks;
      int16_t err = (r - l) * ENC_KOEF;
      setMotors(-1 * (speed - err), -1 * (speed + err));
    }
  }
  stopMotors();
}
void turnAngle(int angle, int speed = 40) {
  leftMotor.resetTicks();
  rightMotor.resetTicks();
  int32_t final_ticks = abs(ROBOT_BASE_LENGHT * angle / 360 * MM_TO_TICK);

  if (angle < 0) {
    while (rightMotor.ticks < final_ticks) {
      int16_t l = -1 * leftMotor.ticks;
      int16_t r = rightMotor.ticks;
      int16_t err = (l - r) * ENC_KOEF;
      setMotors(-1 * speed + err, speed + err);
    }
  } else if (angle > 0) {
    while (leftMotor.ticks < final_ticks) {
      int16_t l = leftMotor.ticks;
      int16_t r = -1 * rightMotor.ticks;
      int16_t err = (l - r) * ENC_KOEF;
      setMotors(speed - err, -1 * speed - err);
    }
  }
  stopMotors();
}
void turnToLine(DirectionTypes dir, int speed = 50) {
  if (dir == LEFT) {
    setMotors(-speed, speed);
    while (!leftLineSensor.onLine()) {}
    setMotors(-speed / 2, speed / 2);
    while (!rightLineSensor.onLine()) {}
  } else {
    setMotors(speed, -speed);
    while (!rightLineSensor.onLine()) {}
    setMotors(speed / 2, -speed / 2);
    while (!leftLineSensor.onLine()) {}
  }
  while (abs(leftLineSensor.read() - rightLineSensor.read()) > 15) {
    int err = (leftLineSensor.read() - rightLineSensor.read()) * 0.5;
    setMotors(-err, err);
  }
  stopMotors();
}
void rideLine(MoveStopTypes stop_type, int32_t argument, MoveAlgoritms movement_type = PD_REG, DistSensorTypes sensor = US) {
  _moveLine(stop_type, argument, movement_type, sensor);
}
void rideWall(int goal_dist, bool reversed, MoveStopTypes stop_type, int32_t argument, DistSensorTypes sensor = US, MoveAlgoritms movement_type = PD_REG) {
  _moveWall(goal_dist, reversed, stop_type, argument, sensor, movement_type);
}
void rideHalf(int speed = 50){
  rideDist(90, speed);
}
void servoClose(){
  servo.write(180);
}
void servoOpen(){
  servo.write(90);
}