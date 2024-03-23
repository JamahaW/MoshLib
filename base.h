#pragma once
#include <Servo.h>

//pins

#define PIN_MOTOR_RIGHT_DIR 4
#define PIN_MOTOR_RIGHT_SPEED 5
#define PIN_MOTOR_LEFT_DIR 7
#define PIN_MOTOR_LEFT_SPEED 6


#define PIN_MOTOR_LEFT_ENC_1 2
#define PIN_MOTOR_RIGHT_ENC_1 3
#define PIN_MOTOR_RIGHT_ENC_2 8
#define PIN_MOTOR_LEFT_ENC_2 9


#define PIN_LINE_LEFT A0
#define PIN_LINE_RIGHT A1

#define PIN_DIST_US_ECHO 11
#define PIN_DIST_US_TRIG 10
#define PIN_DIST_IR A3

#define PIN_SERVO 12

//constants

#define WHITE_RIGHT 270
#define BLACK_RIGHT 985
#define GRAY_RIGHT ((WHITE_RIGHT + BLACK_RIGHT) / 2)

#define WHITE_LEFT 240
#define BLACK_LEFT 990
#define GRAY_LEFT ((WHITE_LEFT + BLACK_LEFT) / 2)

//robot params
#define WHEEL_DIAMETR 68
#define WHEEL_RADIUS (WHEEL_DIAMETR / 2)
#define WHEEL_LENGHT (WHEEL_DIAMETR * PI)

#define ROBOT_BASE_DIAMETR 202
#define ROBOT_BASE_RADIUS (ROBOT_BASE_DIAMETR / 2)
#define ROBOT_BASE_LENGHT (ROBOT_BASE_DIAMETR * PI)

//encoders ticks
#define TICK_PER_ROUND 496
#define TICKS_1000 450
#define TICK_TO_MM (TICKS_1000 / 1000)
#define MM_TO_TICK (1000 / TICKS_1000)

enum MoveStopTypes { ON_TIME,
                     ON_CROSS,
                     BY_RIDE_DISTANCE,
                     ON_WALL_DISTANCE,
                     BY_END_WALL,
                     ON_LINE,
                     NONE };
enum MoveAlgoritms { ZIG_ZAG_LEFT,
                     ZIG_ZAG_RIGHT,
                     ZIG_ZAG_BOTH,
                     ZIG_ZAG,
                     PD_REG };
enum DirectionTypes { LEFT,
                      RIGHT,
                      FORWARD,
                      BACK };
enum DistSensorTypes { IR,
                       US };
enum TestModes { LINE,
                 DIST,
                 ENCODERS,
                 MOTORS };
typedef void (*function)(void);

struct Motor {
  const uint8_t PIN_DIR;
  const uint8_t PIN_SPEED;
  const uint8_t PIN_ENC_A;
  const uint8_t PIN_ENC_B;
  bool reversed;
  volatile int32_t ticks = 0;

  Motor(uint8_t dir, uint8_t speed, uint8_t enc_a, uint8_t enc_b, bool rev)
    : PIN_DIR(dir), PIN_SPEED(speed), PIN_ENC_A(enc_a), PIN_ENC_B(enc_b), reversed(rev) {
    pinMode(PIN_DIR, OUTPUT);
    pinMode(PIN_SPEED, OUTPUT);
    pinMode(PIN_ENC_A, INPUT);
    pinMode(PIN_ENC_B, INPUT);
  }
  void setDir(bool state) {
    digitalWrite(PIN_DIR, state ^ reversed);
  }
  void setSpeed(uint8_t speed) {
    analogWrite(PIN_SPEED, constrain(map(abs(speed), 0, 100, 0, 255), 0, 255));
  }
  void set(int power) {
    setSpeed(power);
    setDir(power < 0);
  }
  void attach(function intteruptHandler) {
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), intteruptHandler, RISING);
  }
  void inc() {
    if (digitalRead(PIN_ENC_B)) {
      ticks++;
    } else {
      ticks--;
    }
  }
  void resetTicks() {
    ticks = 0;
  }};
struct LineSensor {
  const uint8_t PIN;
  const int black;
  const int white;
  const int gray;
  const bool reversed = true;
  LineSensor(uint8_t _p, int _v_line, int v_field, int _g, bool rev)
    : PIN(_p), black(_v_line), white(v_field), gray(_g), reversed(rev) {
    pinMode(PIN, INPUT);
  }
  int readRaw() {
    return analogRead(PIN);
  }
  int read() {
    return constrain(map(readRaw(), white, black, 0, 100), 0, 100);
  }
  bool onLine() {
    return (read() < 50) != reversed;
  }};
static float _getDist_US() {
  delayMicroseconds(10);
  digitalWrite(PIN_DIST_US_TRIG, 1);
  delayMicroseconds(10);
  digitalWrite(PIN_DIST_US_TRIG, 0);
  return pulseIn(PIN_DIST_US_ECHO, 1) / 58.2;}
static float _getDist_IR() {
  float d = 0;
  for (int i = 0; i < 5; i++) {
    d += 32.0 * pow(analogRead(PIN_DIST_IR) * 5.0 / 1024, -1.10);
    delay(1);
  }
  return d / 5;}
float getDist(DistSensorTypes type = US) {
  float dist;
  if (type == IR) {
    dist = _getDist_IR();
  }
  if (type == US) {
    dist = _getDist_US();
  }
  return dist;}