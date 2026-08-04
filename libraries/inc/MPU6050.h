/*
 * MPU6050.h
 *
 *  Created on: Feb 28, 2026
 *      Author: ozker
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_
#include "MPU6050_regs.h"
#include "stdbool.h"
#include "i2c.h"

typedef struct {
    I2C_HandleTypeDef *i2c;

    float accel_divider;
    float gyro_divider;

    float gyro_offsetX;
    float gyro_offsetY;
    float gyro_offsetZ;

    float filteredAngle_deg;
}mpu6050_t;

bool mpu_whoAmI(mpu6050_t *mpu);
void mpu_init(mpu6050_t *mpu, I2C_HandleTypeDef *hi2c, mpu_gyroRange_t gyroRange, mpu_accelRange_t accelRange);

void mpu_getAccel(mpu6050_t *mpu, float *accel_x_ptr, float *accel_y_ptr, float *accel_z_ptr);
void mpu_getGyro(mpu6050_t *mpu, float *gyro_x_ptr, float *gyro_y_ptr, float *gyro_z_ptr);

void mpu_countGyroOffset(mpu6050_t *mpu);

void mpu_getGyroOffset(mpu6050_t *mpu, float *gx_ptr, float *gy_ptr, float *gz_ptr);

//float mpu_countAngle(mpu6050_t *mpu, float accelNumerator, float accelDenominator, float gyroRate, float dt_s);


#endif /* INC_MPU6050_H_ */
