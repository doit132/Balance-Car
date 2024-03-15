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
#include "encoder.h"
#include "motor.h"

/* ANCHOR - 全局变量声明 */

extern float          GyroX, GyroY, GyroZ; /* 陀螺仪数据 */
extern float          AccX, AccY, AccZ;    /* 加速度计数据 */
extern float          Pitch, Yaw, Roll;    /* 欧拉角 */
extern u8             WayAngle;            /* 获得欧拉角的方式: 1: DMP 2: 卡尔曼滤波 3: 互补滤波 */
extern u8             FlagStop;            /* 小车停止标志位 */
extern float          Voltage;             /* 电池电压 */
extern u8             FlagAvoid;           /* 小车避障模式标志位 */
extern u8             FlagFollow;          /* 小车跟随模式标志位 */
extern Hcsr04Info_t   Hcsr04Info;          /* 超声波传感器结构体, 内部有测量距离 */
extern Encoder_Data_t Encoder_Data;        /* 编码器数据 */
extern Motor_Data_t   Motor_Data;          /* 电机数据: 小车行驶速度 */

/* ANCHOR - 公共函数声明 */

void BSP_Init(void);
#ifdef __cplusplus
}
#endif

#endif /* BSP_H */
