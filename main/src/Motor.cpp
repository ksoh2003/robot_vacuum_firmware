#include "../include/Motor.hpp"
#include "driver/gpio.h"
#include <cstdint>
#include "driver/ledc.h"

#define MOTOR_LEFT GPIO_NUM_23
#define MOTOR_RIGHT GPIO_NUM_25
#define MOTOR_MAIN GPIO_NUM_26

#define PWM_PIN GPIO_NUM_32   // MD10C의 PWM 핀과 연결
#define DIR_PIN GPIO_NUM_27   // MD10C의 DIR 핀과 연결

#define PWM_FREQ  20000       // 20kHz (MD10C 최대치)
#define DUTY_100  1024        // 10비트 해상도에서 100%

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

  // DIR 핀: 출력으로 설정하고 Low(정방향)로 고정
  gpio_config_t dir = {};
  dir.pin_bit_mask = 1ULL << DIR_PIN;
  dir.mode = GPIO_MODE_OUTPUT;
  gpio_config(&dir);
  gpio_set_level(DIR_PIN, 0);

  // PWM 타이머 설정
  ledc_timer_config_t timer = {};
  timer.speed_mode      = LEDC_LOW_SPEED_MODE;
  timer.timer_num       = LEDC_TIMER_0;
  timer.duty_resolution = LEDC_TIMER_10_BIT;
  timer.freq_hz         = PWM_FREQ;
  timer.clk_cfg         = LEDC_AUTO_CLK;
  ledc_timer_config(&timer);

  // PWM 채널 설정
  ledc_channel_config_t ch = {};
  ch.gpio_num   = PWM_PIN;
  ch.speed_mode = LEDC_LOW_SPEED_MODE;
  ch.channel    = LEDC_CHANNEL_0;
  ch.timer_sel  = LEDC_TIMER_0;
  ledc_channel_config(&ch);
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

void Motor::VacuumMotor(int percent) {
  if (percent < 0)   percent = 0;
  if (percent > 100) percent = 100;

  int duty = percent * DUTY_100 / 100;
  ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty, 0);
}