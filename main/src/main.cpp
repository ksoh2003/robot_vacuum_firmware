#include <iostream>
#include "../include/Run.hpp"
#include "../include/Mortor.hpp"
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

void RunMortor(void* parameter) {
  auto* mortor = static_cast<Mortor*>(parameter);
  while (1) {
    mortor->MortorRun(1);
    vTaskDelay(60);
  }
}


extern "C" void app_main() {
  std::cout << "Start Logic" << std::endl;
  i2cdev_init();
  static Run run;
  static Mortor mortor;
  // xTaskCreate(GetDistance, "GetDistance", 2048, &run, 5, NULL);
  xTaskCreate(ReadMPU, "ReadMPU", 2048, &run, 5, NULL);
  // xTaskCreate(RunMortor, "RunMortor", 2048, &mortor, 5, NULL);
}