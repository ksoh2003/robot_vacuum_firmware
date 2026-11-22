#include "../include/Motor.hpp"
#include "driver/gpio.h"
#include <cstdint>

#define MOTOR_LEFT GPIO_NUM_23
#define MOTOR_RIGHT GPIO_NUM_25
#define MOTOR_MAIN GPIO_NUM_26

Motor::Motor() {
  gpio_reset_pin(MOTOR_LEFT);
  gpio_reset_pin(MOTOR_RIGHT);
  gpio_reset_pin(MOTOR_MAIN);

  gpio_set_direction(MOTOR_LEFT, GPIO_MODE_OUTPUT);
  gpio_set_direction(MOTOR_RIGHT, GPIO_MODE_OUTPUT);
  gpio_set_direction(MOTOR_MAIN, GPIO_MODE_OUTPUT);

  gpio_set_level(MOTOR_LEFT, 0);
  gpio_set_level(MOTOR_RIGHT, 0);
  gpio_set_level(MOTOR_MAIN, 0);
}

void Motor::MotorRun(int MotorState) {
  static bool stateFront {};
  static bool stateLeft {};
  static bool stateRight {};

  switch (MotorState) {
    case 0:
      stateFront = !stateFront;
      break;
    case 1:
      stateLeft = !stateLeft;
      break;
    case 2:
      stateRight = !stateRight;
      break;
  }

  if (stateLeft) {
    gpio_set_level(MOTOR_LEFT, 1);
    gpio_set_level(MOTOR_RIGHT, 0);
    gpio_set_level(MOTOR_MAIN, 0);
  } else if (stateRight) {
    gpio_set_level(MOTOR_LEFT, 0);
    gpio_set_level(MOTOR_RIGHT, 1);
    gpio_set_level(MOTOR_MAIN, 0);
  } else if (stateFront) {
    gpio_set_level(MOTOR_LEFT, 1);
    gpio_set_level(MOTOR_RIGHT, 1);
    gpio_set_level(MOTOR_MAIN, 1);
  } else {
    gpio_set_level(MOTOR_LEFT, 0);
    gpio_set_level(MOTOR_RIGHT, 0);
    gpio_set_level(MOTOR_MAIN, 0);
  }
}

void Motor::VacuumMotor() {

}