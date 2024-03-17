#ifndef BSP_H
#define BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "key.h"
#include "led.h"
#include "hcsr04.h"
#include "tim.h"
#include "oled.h"
#include "delay.h"
#include "mpu6050.h"
#include "encoder.h"
#include "motor.h"

/* ANCHOR - 全局变量声明 */

extern float          g_gyro_x, g_gyro_y, g_gyro_z; /* 陀螺仪角速度 */
extern float          g_acc_x, g_acc_y, g_acc_z;    /* 加速度计加速度 */
extern float          g_pitch, g_yaw, g_roll;       /* 欧拉角 */
extern short          gyro[3];                      /* 陀螺仪角速度 */
extern short          accel[3];                     /* 加速度计加速度 */
extern float          g_pitch, g_yaw, g_roll;       /* 欧拉角 */
extern float          g_balance_kp, g_balance_kd;   /* 直立环 PD 控制器 */
extern float          g_velocity_kp, g_velocity_ki; /* 速度环 PI 控制器 */
extern u8             g_way_angle;    /* 获得欧拉角的方式: 1: DMP 2: 卡尔曼滤波 3: 互补滤波 */
extern u8             g_flag_stop;    /* 小车停止标志位 */
extern float          g_voltage;      /* 电池电压 */
extern u8             g_flag_avoid;   /* 小车避障模式标志位 */
extern u8             g_flag_follow;  /* 小车跟随模式标志位 */
extern Hcsr04Info_t   g_hcsr04_info;  /* 超声波传感器结构体, 内部有测量距离 */
extern Encoder_Data_t g_encoder_data; /* 编码器数据 */
extern Motor_Data_t   g_motor_data;   /* 电机数据: 小车行驶速度 */
extern u8             g_key_num;      /* 0: 无按键, 1: 短按, 2: 长按, 3: 双击 */

/* ANCHOR - 公共函数声明 */

void BSP_Init(void);
#ifdef __cplusplus
}
#endif

#endif /* BSP_H */
