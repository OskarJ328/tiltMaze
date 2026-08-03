/*
 * MPU6050.c
 *
 *  Created on: Feb 28, 2026
 *      Author: ozker
 */

#include "MPU6050.h"
#include "MPU6050_regs.h"
#include "math.h"
#include "my_delay.h"

static void mpu_read_reg(mpu6050_t *mpu, uint8_t regAddr, uint8_t* resultPtr, uint8_t resultSize){
    HAL_I2C_Mem_Read(mpu->i2c, MPU6050_ADDR, regAddr, 1, resultPtr, resultSize, HAL_MAX_DELAY);
}

static void mpu_write_reg(mpu6050_t *mpu, uint8_t reg_addr, uint8_t write_val){
    HAL_I2C_Mem_Write(mpu->i2c, MPU6050_ADDR, reg_addr, 1, &write_val, 1, HAL_MAX_DELAY);
}

static void mpu_read_accel_raw(mpu6050_t *mpu, int16_t *accelX_rawPtr, int16_t *accelY_rawPtr, int16_t *accelZ_rawPtr){
    uint8_t raw_read[6] = {0};
    mpu_read_reg(mpu, MPU6050_ACCEL_X_OUT_H, raw_read, sizeof(raw_read));
    *accelX_rawPtr = ((int16_t)raw_read[0] << 8) | raw_read[1];
    *accelY_rawPtr = ((int16_t)raw_read[2] << 8) | raw_read[3];
    *accelZ_rawPtr = ((int16_t)raw_read[4] << 8) | raw_read[5]; 
}

static void mpu_read_gyro_raw(mpu6050_t *mpu, int16_t *gyro_x_raw_ptr, int16_t *gyro_y_raw_ptr, int16_t *gyro_z_raw_ptr){
    uint8_t raw_read[6] = {0};
    mpu_read_reg(mpu, MPU6050_GYRO_X_OUT_H, raw_read, sizeof(raw_read));
    *gyro_x_raw_ptr = ((int16_t)raw_read[0] << 8) | raw_read[1];
    *gyro_y_raw_ptr = ((int16_t)raw_read[2] << 8) | raw_read[3];
    *gyro_z_raw_ptr = ((int16_t)raw_read[4] << 8) | raw_read[5];
}

void mpu_init(mpu6050_t *mpu, I2C_HandleTypeDef *hi2c, mpu_gyroRange_t gyroRange, mpu_accelRange_t accelRange){
    mpu->i2c = hi2c;
        switch(gyroRange){
        case GYRO_RANGE_250DPS:
            mpu->gyro_divider = 131.0f;
            break;
        case GYRO_RANGE_500DPS:
            mpu->gyro_divider = 65.5f;
            break;
        case GYRO_RANGE_1000DPS:
            mpu->gyro_divider = 32.8f;
            break;
        case GYRO_RANGE_2000DPS:
            mpu->gyro_divider = 16.4f;
            break;
        default: break;
    }
    switch(accelRange){
        case ACCEL_RANGE_2G:
            mpu->accel_divider = 16384.0f;
            break;
        case ACCEL_RANGE_4G:
            mpu->accel_divider = 8192.0f;
            break;
        case ACCEL_RANGE_8G:
            mpu->accel_divider = 4096.0f;
            break;
        case ACCEL_RANGE_16G:
            mpu->accel_divider = 2048.0f;
            break;
        default: break;
    }
    
    //mpu_gyroRange_t gyro = GYRO_RANGE_500DPS;
    //mpu_accelRange_t accel = ACCEL_RANGE_4G;
    mpu_dlpf_t dlpf = DLPF_BW_42HZ;
    mpu_write_reg(mpu, MPU6050_POWER_MANAGMENT1, WAKE_UP);
    mpu_write_reg(mpu, MPU6050_CONFIG, dlpf);
    mpu_write_reg(mpu, MPU6050_GYRO_CONFIG, gyroRange);
    mpu_write_reg(mpu, MPU6050_ACCEL_CONFIG, accelRange);


}

bool mpu_whoAmI(mpu6050_t *mpu){
    uint8_t result;
    mpu_read_reg(mpu, MPU6050_WHO_AM_I, &result, sizeof(result));
    if(result == 0x68){
        return true;
    }
    return false;
}

void mpu_getAccel(mpu6050_t *mpu, float *accel_x_ptr, float *accel_y_ptr, float *accel_z_ptr){
    int16_t accel_x_raw = 0;
    int16_t accel_y_raw = 0;
    int16_t accel_z_raw = 0;
    mpu_read_accel_raw(mpu, &accel_x_raw, &accel_y_raw, &accel_z_raw);

    *accel_x_ptr = accel_x_raw / mpu->accel_divider;
    *accel_y_ptr = accel_y_raw / mpu->accel_divider;
    *accel_z_ptr = accel_z_raw / mpu->accel_divider;
}

void mpu_getGyro(mpu6050_t *mpu, float *gyro_x_ptr, float *gyro_y_ptr, float *gyro_z_ptr){
    int16_t gyro_x_raw = 0;
    int16_t gyro_y_raw = 0;
    int16_t gyro_z_raw = 0;
    mpu_read_gyro_raw(mpu, &gyro_x_raw, &gyro_y_raw, &gyro_z_raw);

    *gyro_x_ptr = gyro_x_raw / mpu->gyro_divider - mpu->gyro_offsetX;
    *gyro_y_ptr = gyro_y_raw / mpu->gyro_divider - mpu->gyro_offsetY;
    *gyro_z_ptr = gyro_z_raw / mpu->gyro_divider - mpu->gyro_offsetZ;
}


void mpu_countGyroOffset(mpu6050_t *mpu){
    mpu->gyro_offsetX = 0;
    mpu->gyro_offsetY = 0;
    mpu->gyro_offsetZ = 0;

    float gx, gy, gz;
    float sum_x = 0.0f;
    float sum_y = 0.0f;
    float sum_z = 0.0f;
    uint16_t num_samples = 500;

    for(uint16_t i = 0; i<num_samples; i++){
        mpu_getGyro(mpu, &gx, &gy, &gz);
        sum_x += gx;
        sum_y += gy;
        sum_z += gz;
        myDelay(5);
    }
    mpu->gyro_offsetX = sum_x / num_samples;
    mpu->gyro_offsetY = sum_y / num_samples;
    mpu->gyro_offsetZ = sum_z / num_samples;
}

void mpu_getGyroOffset(mpu6050_t *mpu, float *gx_ptr, float *gy_ptr, float *gz_ptr){
    *gx_ptr = mpu->gyro_offsetX;
    *gy_ptr = mpu->gyro_offsetY;
    *gz_ptr = mpu->gyro_offsetZ;
}
/*
float mpu_countAngle(mpu6050_t *mpu, float accelNumerator, float accelDenominator, float gyroRate, float dt_s){

    float accel_angleDeg = atan2(accelNumerator, accelDenominator) * (180.0f / 3.141592f);

    mpu->filteredAngle_deg = 0.98f * (mpu->filteredAngle_deg + (gyroRate * dt_s)) + 0.02f * accel_angleDeg;

    return mpu->filteredAngle_deg;
}
*/
