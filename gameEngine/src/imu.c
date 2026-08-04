#include "imu.h"
#include "MPU6050.h"
#include "math.h"

bool imu_init(imu_t *imu, mpu6050_t *mpu, I2C_HandleTypeDef *hi2c, mpu_gyroRange_t gyroRange, mpu_accelRange_t accelRange, float complementary_accelWeight){
    imu->mpu = mpu;
    imu->accel_g.x = 0;
    imu->accel_g.y = 0;
    imu->accel_g.z = 0;
    imu->gyro_deg_s.x = 0;
    imu->gyro_deg_s.y = 0;
    imu->gyro_deg_s.z = 0;
    imu->orientation.pitch = 0;
    imu->orientation.yaw = 0;
    imu->orientation.roll = 0;
    
    imu->complementary_accelWeight = complementary_accelWeight;
    imu->complementary_gyroWeight = 1.00f - complementary_accelWeight;


    mpu_init(mpu, hi2c, gyroRange, accelRange);
    if(!mpu_whoAmI(mpu)){
        return false;
    }
    mpu_countGyroOffset(mpu);
    return true;
}

static float complementary_countAngle(imu_t *imu, float prevFilteredAngle, float accelNumerator, float accelDenominator, float gyroRate, float dt_s){
    float accel_angleDeg = atan2(accelNumerator, accelDenominator) * (180.0f / 3.141592f);

    float newFilteredAngle = imu->complementary_gyroWeight * (prevFilteredAngle + (gyroRate * dt_s)) + imu->complementary_accelWeight * accel_angleDeg; 

    return newFilteredAngle;
}

static void imu_countPitch(imu_t *imu){
    
}

static void imu_countRoll(imu_t *imu){

}

static void imu_countYaw(imu_t *imu){

}

void imu_update(imu_t *imu){
    mpu_getAccel(imu->mpu, &imu->accel_g.x, &imu->accel_g.y, &imu->accel_g.z);
    mpu_getGyro(imu->mpu, &imu->gyro_deg_s.x, &imu->gyro_deg_s.y, &imu->gyro_deg_s.z);
    imu_countPitch(imu);
    imu_countRoll(imu);
    imu_countYaw(imu);
}

floatVector3_t imu_getAccel(imu_t *imu){
    return imu->accel_g;
}
floatVector3_t imu_getGyro(imu_t *imu){
    return imu->gyro_deg_s;
}
orientation_t imu_getOrientation(imu_t *imu){
    return imu->orientation;
}

