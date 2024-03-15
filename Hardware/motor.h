#ifndef MOTOR_H
#define MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "main.h"
#include "sys.h"
#include "encoder.h"

/* ANCHOR - 结构体声明 */

typedef struct
{
    float Velocity_Left;  /* 左电机线速度 单位: mm/s */
    float Velocity_Right; /* 右电机线速度 单位: mm/s */
} Motor_Data_t;           /* 输出的电机数据 */

/* ANCHOR - 宏定义 */

/* 用来计算小车行驶速度的参数 */
#define PI                3.14159265 /* PI 圆周率 */
#define CONTROL_FREQUENCY 200.0      /* 编码器读取频率 5ms 一次 */
#define ENCODER_MULTIPLES 4.0        /* 编码器倍频数 */
#define ENCODER_PRECISION 13.0       /* 编码器精度 13 线, 一圈: 13 个脉冲数量 */
#define REDUCTION_RATIO   30.0       /* 减速比 30 */
#define DIAMETER_67       67.0       /* 轮子直径 67mm */

/* ANCHOR - 公共函数声明 */

void Motor_Set_Pwm(int motor_left, int motor_right);
void Motor_TurnOff(u8 motor);
void Motor_Init(Motor_Data_t* pData);
void Motor_Get_Velocity_From_Encoder(Motor_Data_t* pMotorData, Encoder_Data_t* pEncoderData);
#ifdef __cplusplus
}
#endif

#endif /* MOTOR_H */
