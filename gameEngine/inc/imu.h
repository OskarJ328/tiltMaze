#pragma once

#include "MPU6050.h"
#include "basicTypes.h"


typedef struct{
    float pitch;
    float roll;
    float yaw;
}orientation_t;

typedef struct{
    mpu6050_t *mpu;
    floatVector3_t accel_g;
    floatVector3_t gyro_deg_s;
    orientation_t orientation;
    float complementary_accelWeight;
    float complementary_gyroWeight;
}imu_t;

bool imu_init(imu_t *imu, mpu6050_t *mpu, I2C_HandleTypeDef *hi2c, mpu_gyroRange_t gyroRange, mpu_accelRange_t accelRange, float complementary_accelWeight);
void imu_update(imu_t *imu);

floatVector3_t imu_getAccel(imu_t *imu);
floatVector3_t imu_getGyro(imu_t *imu);
orientation_t imu_getOrientation(imu_t *imu);


