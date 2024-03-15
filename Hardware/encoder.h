#ifndef ENCODER_H
#define ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "main.h"
#include "sys.h"

/* ANCHOR - 宏定义 */

/* ANCHOR - 结构体声明 */

typedef struct Encoder_Data
{
    int encoder_count_l; /* 左电机编码器计数 */
    int encoder_count_r; /* 右电机编码器计数 */
} Encoder_Data_t;

/* ANCHOR - 公共函数声明 */
int  Read_Encoder(u8 TIMX);
void Encoder_Get_Data(Encoder_Data_t* pData);
void Encoder_Init(Encoder_Data_t* pData);
void Encoder_Change_Mode(Encoder_Data_t* pData);
#ifdef __cplusplus
}
#endif

#endif /* MOTOR_H */
