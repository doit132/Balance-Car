#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "hcsr04.h"
#include "delay.h"

/* ANCHOR - 全局变量定义 */

extern Hcsr04Info_t Hcsr04Info;

/* ANCHOR - 公共函数定义 */

void HCSR04_Start(void)
{
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
    Systick_Delay_Us(20); /* 阻塞延时, 不能在中断中使用, 因为 Systick 的中断优先级最低 */
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);
}

void HCSR04_Init(TIM_HandleTypeDef* htim, uint32_t Channel, Hcsr04Info_t* info)
{
    /* 数据初始化 */
    info->period = htim->Init.Period;       /* 65535 */
    info->prescaler = htim->Init.Prescaler; /* 72-1 */
    info->edge_state = 0;
    info->tim_overflow_counter = 0;
    info->distance = 0.0f;
    info->instance = htim->Instance; /* TIM3 */
    info->ic_tim_ch = Channel;       /* Channel3 */

    if (info->ic_tim_ch == TIM_CHANNEL_1)
    {
        info->active_channel = HAL_TIM_ACTIVE_CHANNEL_1;
    }
    else if (info->ic_tim_ch == TIM_CHANNEL_2)
    {
        info->active_channel = HAL_TIM_ACTIVE_CHANNEL_2;
    }
    else if (info->ic_tim_ch == TIM_CHANNEL_3)
    {
        info->active_channel = HAL_TIM_ACTIVE_CHANNEL_3;
    }
    else if (info->ic_tim_ch == TIM_CHANNEL_4)
    {
        info->active_channel = HAL_TIM_ACTIVE_CHANNEL_4;
    }
    else if (info->ic_tim_ch == TIM_CHANNEL_4)
    {
        info->active_channel = HAL_TIM_ACTIVE_CHANNEL_4;
    }

    /* 开启定时器更新中断和捕获中断 */
    HAL_TIM_Base_Start_IT(htim);
    HAL_TIM_IC_Start_IT(htim, Channel);
}

/**
 * *****************************************************************************
 * @brief 读取超声波模块测量的距离
 * @return 
 * *****************************************************************************
 */
void Hcsr04_Get_Data(Hcsr04Info_t* Hcsr04Info)
{
    /* 测距结果限幅 */
    if (Hcsr04Info->distance >= 450)
    {
        Hcsr04Info->distance = 450;
    }
}

/**
 * *****************************************************************************
 * @brief 定时器计数溢出中断处理函数
 * @param [in] htim 
 * *****************************************************************************
 */
void Hcsr04_TIM_Overflow_ISR(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == Hcsr04Info.instance) /* TIM3 */
    {
        Hcsr04Info.tim_overflow_counter++;
    }
}

/**
 * *****************************************************************************
 * @brief 输入捕获计算高电平时间->距离
 * @param [in] htim 
 * *****************************************************************************
 */
void Hcsr04_TIM_IC_ISR(TIM_HandleTypeDef* htim)
{
    if ((htim->Instance == Hcsr04Info.instance) && (htim->Channel == Hcsr04Info.active_channel))
    {
        if (Hcsr04Info.edge_state == 0) /* 捕获上升沿 */
        {
            /* 得到上升沿时, t1 = 计数器的值, 并更改输入捕获为下降沿 */
            Hcsr04Info.t1 = HAL_TIM_ReadCapturedValue(htim, Hcsr04Info.ic_tim_ch);
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, Hcsr04Info.ic_tim_ch, TIM_INPUTCHANNELPOLARITY_FALLING);
            Hcsr04Info.tim_overflow_counter = 0; /* 定时器溢出计数器清零 */
            Hcsr04Info.edge_state = 1;           /* 上升沿, 下降沿捕获标志位 */
        }
        else if (Hcsr04Info.edge_state == 1) /* 捕获下降沿 */
        {
            /* 捕获下降沿时, 计数器的值 */
            Hcsr04Info.t2 = HAL_TIM_ReadCapturedValue(htim, Hcsr04Info.ic_tim_ch);
            /* 需要考虑定时器溢出中断 */
            Hcsr04Info.t2 += Hcsr04Info.tim_overflow_counter * Hcsr04Info.period;
            /* 高电平持续时间 = 下降沿时间点 - 上升沿时间点 */
            Hcsr04Info.high_level_us = Hcsr04Info.t2 - Hcsr04Info.t1;
            /* 计算距离 */
            Hcsr04Info.distance = (Hcsr04Info.high_level_us / 1000000.0) * 340.0 / 2.0 * 100.0; /* 单位: cm */
            /* 重新开启上升沿捕获 */
            Hcsr04Info.edge_state = 0; /* 一次采集完毕, 清零 */
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, Hcsr04Info.ic_tim_ch, TIM_INPUTCHANNELPOLARITY_RISING);
        }
    }
}

#ifdef __cplusplus
}
#endif
