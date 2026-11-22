#include <iostream>
#include "../include/Run.hpp"
#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include "esp_timer.h"
#include <cstdint>
#include <cmath>

/*
  * GPIO LIST *
  - GPIO4
  - GPIO13
  - GPIO14
  - GPIO16
  - GPIO17
  - GPIO18
  - GPIO19
  - GPIO21
  - GPIO22
  - GPIO23 (모터)
  - GPIO25 ("")
  - GPIO26 (모터)
  - GPIO27 (흡입 모터 DIR, 0 & 1로 동작 방향 결정)
  - GPIO32 (흡입 모터 PWM)
  GPIO33
 */

#define SWITCH GPIO_NUM_4
#define HC1_TRIG GPIO_NUM_13
#define HC1_ECHO GPIO_NUM_14
#define HC2_TRIG GPIO_NUM_16
#define HC2_ECHO GPIO_NUM_17
#define HC3_TRIG GPIO_NUM_18
#define HC3_ECHO GPIO_NUM_19
#define MPU_SDA GPIO_NUM_21
#define MPU_SCL GPIO_NUM_22
#define VM_DIR GPIO_NUM_27
#define VM_PWM GPIO_NUM_32

Run::Run() {
  gpio_reset_pin(SWITCH);
  gpio_reset_pin(HC1_TRIG);
  gpio_reset_pin(HC1_ECHO);
  gpio_reset_pin(HC2_TRIG);
  gpio_reset_pin(HC2_ECHO);
  gpio_reset_pin(HC3_TRIG);
  gpio_reset_pin(HC3_ECHO);
  gpio_reset_pin(VM_DIR);
  gpio_reset_pin(VM_PWM);

  gpio_set_direction(SWITCH, GPIO_MODE_INPUT);
  gpio_set_direction(HC1_TRIG, GPIO_MODE_OUTPUT);
  gpio_set_direction(HC1_ECHO, GPIO_MODE_INPUT);
  gpio_set_direction(HC2_TRIG, GPIO_MODE_OUTPUT);
  gpio_set_direction(HC2_ECHO, GPIO_MODE_INPUT);
  gpio_set_direction(HC3_TRIG, GPIO_MODE_OUTPUT);
  gpio_set_direction(HC3_ECHO, GPIO_MODE_INPUT);

  gpio_set_level(HC1_TRIG, 0);
  gpio_set_level(HC2_TRIG, 0);
  gpio_set_level(HC3_TRIG, 0);

  mpu6050_init_desc(&mpu, MPU6050_I2C_ADDRESS_LOW, I2C_NUM_0, MPU_SDA, MPU_SCL);
  mpu6050_init(&mpu);

  float accel_bias {}, gyro_bias {};
  mpu6050_calibrate(&mpu, &accel_bias, &gyro_bias);

  heading_last_us = esp_timer_get_time();
}

void Run::HC_SRO4() {
  // 1번 초음파 센서 측정
  gpio_set_level(HC1_TRIG, 1);
  esp_rom_delay_us(10);
  gpio_set_level(HC1_TRIG, 0);

  // 센서에서 초음파를 발생시키면 자동으로 ECHO가 0으로 됨
  while (gpio_get_level(HC1_ECHO) == 0);

  std::int64_t hc1_echo_start = esp_timer_get_time();
  std::int64_t hc1_echo_middle {};
  double hc1_distance {};

  while (gpio_get_level(HC1_ECHO) == 1) {
    hc1_echo_middle = esp_timer_get_time();
    if (hc1_echo_middle - hc1_echo_start >= 30000) break;
  }

  if (hc1_echo_middle - hc1_echo_start >= 30000) {
    std::cout << "HC1 High Distance" << std::endl;
  } else {
    std::int64_t hc1_echo_end = esp_timer_get_time();
    hc1_distance = (hc1_echo_end - hc1_echo_start) * 0.03432 / 2;
  }

  // 2번 초음파 센서 측정
  gpio_set_level(HC2_TRIG, 1);
  esp_rom_delay_us(10);
  gpio_set_level(HC2_TRIG, 0);

  while (gpio_get_level(HC2_ECHO) == 0);

  std::int64_t hc2_echo_start = esp_timer_get_time();
  std::int64_t hc2_echo_middle {};
  double hc2_distance {};

  while (gpio_get_level(HC2_ECHO) == 1) {
    hc2_echo_middle = esp_timer_get_time();
    if (hc2_echo_middle - hc2_echo_start >= 30000) break;
  }

  if (hc2_echo_middle - hc2_echo_start >= 30000) {
    std::cout << "HC2 High Distance" << std::endl;
  } else {
    std::int64_t hc2_echo_end = esp_timer_get_time();
    hc2_distance = (hc2_echo_end - hc2_echo_start) * 0.03432 / 2;
  }

  // 3번 초음파 센서 측정
  gpio_set_level(HC3_TRIG, 1);
  esp_rom_delay_us(10);
  gpio_set_level(HC3_TRIG, 0);

  while (gpio_get_level(HC3_ECHO) == 0);

  std::int64_t hc3_echo_start = esp_timer_get_time();
  std::int64_t hc3_echo_middle {};
  double hc3_distance {};

  while (gpio_get_level(HC3_ECHO) == 1) {
    hc3_echo_middle = esp_timer_get_time();
    if (hc3_echo_middle - hc3_echo_start >= 30000) break;
  }

  if (hc3_echo_middle - hc3_echo_start >= 30000) {
    std::cout << "HC3 High Distance" << std::endl;
  } else {
    std::int64_t hc3_echo_end = esp_timer_get_time();
    hc3_distance = (hc3_echo_end - hc3_echo_start) * 0.03432 / 2;
  }

  // 양 옆 측정 결과 출력
  std::cout << "L: " << hc1_distance << "  " << "F: " << hc3_distance << "  " << "R: " << hc2_distance << std::endl;

}

void Run::MPU_6050() {
  mpu6050_acceleration_t accel {};
  mpu6050_rotation_t gyro {};

  mpu6050_get_motion(&mpu, &accel, &gyro);

  std::int64_t now_us = esp_timer_get_time();
  double dt = (now_us - heading_last_us) / 1000000.0;  // 지난 호출과의 시간 간격 (초)
  heading_last_us = now_us;

  if (std::abs(gyro.z) > 1.0) heading += gyro.z * dt;
  if (heading >= 360.0) heading -= 360.0;
  if (heading < 0.0) heading += 360.0;

  std::cout << "Heading: " << heading << std::endl;
}