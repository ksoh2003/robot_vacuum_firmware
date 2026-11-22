#pragma once
#include "mpu6050.h"

class Run {
  public:
    Run();
    void HC_SRO4();
    void MPU_6050();

  private:
    mpu6050_dev_t mpu {};
    std::int64_t heading_last_us {};
    double heading {};
};