#ifndef HCSR04_H
#define HCSR04_H

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "sys.h"
#include "main.h"

/* ANCHOR - 结构体声明 */

typedef struct
{
    u8                    edge_state;           /* 记录上升沿和下降沿状态 */
    u16                   tim_overflow_counter; /* 定时器溢出次数 */
    float                 distance;             /* 测量距离 */
    u32                   prescaler;
    u32                   period;
    u32                   t1;            /* 上升沿时间 */
    u32                   t2;            /* 下降沿时间 */
    u32                   high_level_us; /* 高电平持续时间 */
    TIM_TypeDef*          instance;
    u32                   ic_tim_ch;
    HAL_TIM_ActiveChannel active_channel;
} Hcsr04Info_t;

/* ANCHOR - 宏定义 */

/* ANCHOR - 公共函数声明 */
void HCSR04_Init(TIM_HandleTypeDef* htim, uint32_t Channel, Hcsr04Info_t* info);
void HCSR04_Start(void);
void Hcsr04_Get_Data(Hcsr04Info_t* g_hcsr04_info);

/* 中断处理函数/回调函数 */

void Hcsr04_TIM_Overflow_ISR(TIM_HandleTypeDef* htim);
void Hcsr04_TIM_IC_ISR(TIM_HandleTypeDef* htim);

#ifdef __cplusplus
}
#endif

#endif /* HCSR04_H */
