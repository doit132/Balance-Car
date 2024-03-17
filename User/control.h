#ifndef CONTROL_H
#define CONTROL_H

/* ANCHOR - 头文件包含 */

#include "sys.h"
#include "bsp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 公共函数声明 */
int  Control_Check_Pick_Up(Encoder_Data_t* encoder_data);
int  Control_Check_Put_Down(Encoder_Data_t* encoder_data);
int  PWM_Limit(int IN, int max, int min);
u8   Check_Exception(void);
void Car_Mode_Choose(Encoder_Data_t* encoder_data);
int  Control_Balance_PD(float angle, float mechanical_balance, float gyro);
int  Control_Velocity_PI(Encoder_Data_t* encoder_data);
#ifdef __cplusplus
}
#endif

#endif /* CONTROL_H */
