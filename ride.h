#include <stdint.h>
#pragma once
#include "base.h"

#define CROSSES_DELAY 300

#define BASE_SPEED 30

#define P_KOEF 0.4
#define D_KOEF 0.3
#define I_KOEF 0

#define ENC_KOEF 5

uint8_t cur_crosses = 0;
uint32_t cross_timer = millis();
bool cross_flag = true;
uint32_t start_time = millis();

Motor leftMotor(PIN_MOTOR_LEFT_DIR, PIN_MOTOR_LEFT_SPEED, PIN_MOTOR_LEFT_ENC_1, PIN_MOTOR_LEFT_ENC_2, false);
Motor rightMotor(PIN_MOTOR_RIGHT_DIR, PIN_MOTOR_RIGHT_SPEED, PIN_MOTOR_RIGHT_ENC_1, PIN_MOTOR_RIGHT_ENC_2, true);
LineSensor leftLineSensor(PIN_LINE_LEFT, BLACK_LEFT, WHITE_LEFT, GRAY_LEFT, false);
LineSensor rightLineSensor(PIN_LINE_RIGHT, BLACK_RIGHT, WHITE_RIGHT, GRAY_RIGHT, false);
Servo servo;

static void _leftDrive() {
  leftMotor.inc();}
static void _rightDrive() {
  rightMotor.inc();}
void init_motors() {
  leftMotor.attach(_leftDrive);
  rightMotor.attach(_rightDrive);}
void resetTicks() {
  leftMotor.resetTicks();
  rightMotor.resetTicks();}

//функции
void setMotors(int16_t left_speed, int16_t right_speed, uint32_t time = 0) {  //управление моторами
  leftMotor.set(left_speed);
  rightMotor.set(right_speed);
  delay(time);}
void betterSetMotors(int16_t left_speed, int16_t right_speed){
  
}
void stopMotors(uint16_t timer = 500) {  //торможение
  resetTicks();
  uint16_t start = millis();
  while (millis() - start < timer) {
    int16_t l = leftMotor.ticks;
    int16_t r = rightMotor.ticks;
    setMotors(-l, -r);
  }
  setMotors(0, 0);}

//езда по линии
static void _moveLineTickPD() {  //PD-регулятор
  static int old_error = 0;
  float err, uP, uD, u;

  err = leftLineSensor.read() - rightLineSensor.read();
  uP = err * P_KOEF;
  uD = (err - old_error) * D_KOEF;
  u = uP + uD;

  setMotors(BASE_SPEED + u, BASE_SPEED - u);

  old_error = err;
}
static void _moveLineTickZigZagLeft() {  //заг-заг по левому датчику
  if (leftLineSensor.onLine()) {
    setMotors(-40, 100);
  } else {
    setMotors(100, -40);
  }
}
static void _moveLineTickZigZagRight() {  //зиг-заг по правому датчику
  if (rightLineSensor.onLine()) {
    setMotors(100, -40);
  } else {
    setMotors(-40, 100);
  }
}
static void _moveLineTickZigZagBoth() {  //зиг-заг по 2 датчикам
  bool r = rightLineSensor.onLine();
  bool l = leftLineSensor.onLine();
  if (l == r) {
    setMotors(100, 100);
  } else if (r) {
    setMotors(100, -70);
  } else {
    setMotors(-70, 100);
  }
}
static void _moveWallTickPD(float target, DistSensorTypes sensor, bool reversed) {
  static int old_error = 0;
  float err, uP, uD, u;

  err = target - getDist(sensor);
  uP = err * P_KOEF;
  uD = (err - old_error) * D_KOEF;
  u = uP + uD;
  old_error = err;
  if (reversed) u = u * -1;
  setMotors(BASE_SPEED + u, BASE_SPEED - u);
}
static void _moveWallTickZigZag(float target, DistSensorTypes sensor, bool reversed) {
  if (abs(target - getDist(sensor)) < 4) {
    setMotors(100, 100);
  } else if (reversed) {
    if (target > sensor) {
      setMotors(100, -50);
    } else {
      setMotors(-50, 100);
    }
  } else {
    if (target > sensor) {
      setMotors(-50, 100);
    } else {
      setMotors(100, -50);
    }
  }
}

static bool _timeStopCondition(uint32_t finish_time) {  //остановка по времени
  return millis() <= finish_time;
}
static bool _crossesStopCondition(int end_crosses) {  //остановка по количеству перекрестков
  bool r = rightLineSensor.onLine();
  bool l = leftLineSensor.onLine();
  if ((millis() - cross_timer > CROSSES_DELAY) && r && l && (cross_flag)) {
    cur_crosses++;
    cross_timer = millis();
    cross_flag = !cross_flag;
  }
  if ((millis() - cross_timer > CROSSES_DELAY) && (!(l && r)) && (!cross_flag)) {
    cross_timer = millis();
    cross_flag = !cross_flag;
  }
  return cur_crosses <= end_crosses;
}
static bool _distanceStopCondition(int end_distance) {  //остановка по проеханной дистанции
  int lenght = min(leftMotor.ticks, rightMotor.ticks) * TICK_TO_MM;
  return lenght < end_distance;
}
static bool _distanceSensorStopCondition(int distance, DistSensorTypes sensor = US) {  //остановка по датчику расстояния
  return getDist(sensor) < distance;
}
static bool _endWallStopCondition(DistSensorTypes sensor, int32_t dist) {
  return getDist(sensor) > dist;
}
static bool _lineSensorStopCondition() {
  return !(leftLineSensor.onLine() || rightLineSensor.onLine());
}

static void _moveLineTick(MoveAlgoritms movement_type) {  //корректировка моторов при езде по линии
  switch (movement_type) {
    case PD_REG:
      _moveLineTickPD();
      break;
    case ZIG_ZAG_LEFT:
      _moveLineTickZigZagLeft();
      break;
    case ZIG_ZAG_RIGHT:
      _moveLineTickZigZagRight();
      break;
    case ZIG_ZAG_BOTH:
      _moveLineTickZigZagBoth();
      break;
    default:
      break;
  }
}
static void _moveWallTick(float target, MoveAlgoritms movement_type, DistSensorTypes sensor, bool reversed) {
  switch (movement_type) {
    case PD_REG:
      _moveWallTickPD(target, sensor, reversed);
      break;
    case ZIG_ZAG:
      _moveWallTickZigZag(target, sensor, reversed);
      break;
    default:
      break;
  }
}

static bool _checkMoveConditions(MoveStopTypes stop_type, int32_t argument, DistSensorTypes dist = US) {  //проверка условий остановки
  switch (stop_type) {
    case ON_TIME:
      return _timeStopCondition(start_time + argument);
      break;
    case ON_CROSS:
      return _crossesStopCondition(argument);
      break;
    case BY_RIDE_DISTANCE:
      return _distanceStopCondition(argument);
      break;
    case ON_WALL_DISTANCE:
      return _distanceSensorStopCondition(argument, dist);
      break;
    default:
      return true;
      break;
  }
}
static bool _checkMoveConditions_Wall(MoveStopTypes stop_type, int32_t argument, DistSensorTypes dist) {
  switch (stop_type) {
    case ON_TIME:
      return _timeStopCondition(start_time + argument);
      break;
    case ON_LINE:
      return _lineSensorStopCondition();
      break;
    case BY_RIDE_DISTANCE:
      return _distanceStopCondition(argument);
      break;
    case BY_END_WALL:
      return _endWallStopCondition(dist, argument);
      break;
    default:
      return true;
      break;
  }
}

void _moveLine(MoveStopTypes stop_type = NONE, int32_t argument = 0, MoveAlgoritms movement_type = PD_REG, DistSensorTypes dist = US) {  //полная функция езды по линии
  leftMotor.resetTicks();
  rightMotor.resetTicks();
  start_time = millis();
  cur_crosses = 0;

  while (_checkMoveConditions(stop_type, argument, dist)) {
    _moveLineTick(movement_type);
  }
  stopMotors();
}
void _moveWall(float target, bool reversed = true, MoveStopTypes stop_type = NONE, int32_t argument = 0, DistSensorTypes dist = US, MoveAlgoritms movement_type = PD_REG) {
  leftMotor.resetTicks();
  rightMotor.resetTicks();
  start_time = millis();

  while (_checkMoveConditions_Wall(stop_type, argument, dist)) {
    _moveWallTick(target, movement_type, dist, reversed);
  }
  stopMotors();
}