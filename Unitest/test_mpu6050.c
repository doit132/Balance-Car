#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "test_mpu6050.h"
#include "mpu6050.h"
#include "delay.h"

/* ANCHOR - 全局变量定义 */

extern MPU6050_Data_t MPU6050_Data;

/* ANCHOR - 公共函数定义 */

void Test_MPU6050_Custom(void)
{
    MPU6050_Init(&MPU6050_Data);
    while (1)
    {
        MPU6050_Get_Data(&MPU6050_Data);
        MPU6050_Get_Angle(&MPU6050_Data, 0);
        printf("------------------------------\r\n");
        printf("acc_x = %f\r\n", MPU6050_Data.acc_x);
        printf("acc_y = %f\r\n", MPU6050_Data.acc_y);
        printf("acc_z = %f\r\n", MPU6050_Data.acc_z);
        printf("gyro_x = %f\r\n", MPU6050_Data.gyro_x);
        printf("gyro_y = %f\r\n", MPU6050_Data.gyro_y);
        printf("gyro_z = %f\r\n", MPU6050_Data.gyro_z);
        printf("temp = %f\r\n", MPU6050_Data.temp);
        printf("pitch = %f\r\n", MPU6050_Data.pitch);
        printf("roll = %f\r\n", MPU6050_Data.roll);
        printf("yaw = %f\r\n", MPU6050_Data.yaw);
        printf("------------------------------\r\n");
        HAL_Delay(500);
    }
}

void Test_MPU6050_DMP(void)
{
    int   ret = 0;
    float pitch, roll, yaw;
    do
    {
        ret = MPU6050_DMP_Init();
        printf("MPU6050 init, ret = %d\r\n", ret);
        HAL_Delay(500);
    } while (ret);

    while (1)
    {
        if (MPU6050_DMP_Get_Data(&pitch, &roll, &yaw) == 0)
        {
            printf("pitch = %f, roll = %f, yaw = %f\r\n", pitch, roll, yaw);
        }
        Systick_Delay_Ms(10);
    }
}

void Test_MPU6050(void)
{
    // Test_MPU6050_Custom();
    Test_MPU6050_DMP();
}
#ifdef __cplusplus
}
#endif
