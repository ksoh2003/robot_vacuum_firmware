#include <iostream>
#include "../include/Run.hpp"
#include "../include/Motor.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void GetDistance(void* parameter) {
  auto* run = static_cast<Run*>(parameter);
  while (1) {
    run->HC_SRO4();
    vTaskDelay(60);
  }
}

void ReadMPU(void* parameter) {
  auto* run = static_cast<Run*>(parameter);
  while (1) {
    run->MPU_6050();
    vTaskDelay(60);
  }
}

void RunMotor(void* parameter) {
  auto* motor = static_cast<Motor*>(parameter);
  while (1) {
    motor->MotorRun(1);
    vTaskDelay(60);
  }
}


extern "C" void app_main() {
  std::cout << "Start Logic" << std::endl;
  i2cdev_init();
  static Run run;
  static Motor motor;
  // xTaskCreate(GetDistance, "GetDistance", 2048, &run, 5, NULL);
  xTaskCreate(ReadMPU, "ReadMPU", 2048, &run, 5, NULL);
  // xTaskCreate(RunMotor, "RunMotor", 2048, &motor, 5, NULL);
}