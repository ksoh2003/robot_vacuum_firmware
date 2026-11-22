#include "../include/Mortor.hpp"
#include "driver/gpio.h"
#include <cstdint>

#define MORTOR_LEFT GPIO_NUM_23
#define MORTOR_RIGHT GPIO_NUM_25
#define MORTOR_MAIN GPIO_NUM_26

Mortor::Mortor() {
  gpio_reset_pin(MORTOR_LEFT);
  gpio_reset_pin(MORTOR_RIGHT);
  gpio_reset_pin(MORTOR_MAIN);

  gpio_set_direction(MORTOR_LEFT, GPIO_MODE_OUTPUT);
  gpio_set_direction(MORTOR_RIGHT, GPIO_MODE_OUTPUT);
  gpio_set_direction(MORTOR_MAIN, GPIO_MODE_OUTPUT);

  gpio_set_level(MORTOR_LEFT, 0);
  gpio_set_level(MORTOR_RIGHT, 0);
  gpio_set_level(MORTOR_MAIN, 0);
}

void Mortor::MortorRun(int MortorState) {
  static bool stateFront {};
  static bool stateLeft {};
  static bool stateRight {};

  switch (MortorState) {
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
    gpio_set_level(MORTOR_LEFT, 1);
    gpio_set_level(MORTOR_RIGHT, 0);
    gpio_set_level(MORTOR_MAIN, 0);
  } else if (stateRight) {
    gpio_set_level(MORTOR_LEFT, 0);
    gpio_set_level(MORTOR_RIGHT, 1);
    gpio_set_level(MORTOR_MAIN, 0);
  } else if (stateFront) {
    gpio_set_level(MORTOR_LEFT, 1);
    gpio_set_level(MORTOR_RIGHT, 1);
    gpio_set_level(MORTOR_MAIN, 1);
  } else {
    gpio_set_level(MORTOR_LEFT, 0);
    gpio_set_level(MORTOR_RIGHT, 0);
    gpio_set_level(MORTOR_MAIN, 0);
  }
}

void Mortor::VacuumMortor() {

}