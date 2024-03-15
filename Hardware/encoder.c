#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "tim.h"
#include "encoder.h"
#include "sys.h"

/* ANCHOR - 公共函数定义 */

/**
 * *****************************************************************************
 * @brief 读取编码器的计数器数值
 * @param [in] TIMX 读哪个定时器 
 * @note - TIMX 的取值
 * @note    - 2: TIM2
 * @note    - 3: TIM3
 * @note    - 4: TIM4
 * @return 将编码器计数器的数值返回
 * *****************************************************************************
 */
int Read_Encoder(u8 TIMX)
{
    /* 检查 TIMX 的取值 */
    assert(TIMX == 2 || TIMX == 3 || TIMX == 4);

    int Encoder_TIM;
    switch (TIMX)
    {
        case 2:
            Encoder_TIM = -(short)__HAL_TIM_GetCounter(&htim2);
            __HAL_TIM_SetCounter(&htim2, 0);
            break;
        case 3:
            Encoder_TIM = -(short)__HAL_TIM_GetCounter(&htim3);
            __HAL_TIM_SetCounter(&htim3, 0);
            break;
        case 4:
            Encoder_TIM = -(short)__HAL_TIM_GetCounter(&htim4);
            __HAL_TIM_SetCounter(&htim4, 0);
            break;
        default:
            Encoder_TIM = 0;
    }
    return Encoder_TIM;
}

void Encoder_Init(Encoder_Data_t* pData)
{
    /* 防御式编程, 检查指针是否为空 */
    assert(pData != NULL);

    pData->encoder_count_l = 0;
    pData->encoder_count_r = 0;
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1 | TIM_CHANNEL_2);
}

/**
 * *****************************************************************************
 * @brief 获取编码器数据
 * @param [out] pData 指向存储编码器数据的结构体
 * *****************************************************************************
 */
void Encoder_Get_Data(Encoder_Data_t* pData)
{
    /* 防御式编程, 检查指针是否为空 */
    assert(pData != NULL);

    pData->encoder_count_l = Read_Encoder(2);
    pData->encoder_count_r = Read_Encoder(4);
}

/**
 * *****************************************************************************
 * @brief 选中小车右轮, 改变小车的运动模式
 * @param [in] pData 指向存储编码器数据的结构体
 * *****************************************************************************
 */
void Encoder_Change_Mode(Encoder_Data_t* pData)
{
    extern u8  FlagStop, FlagFollow, FlagAvoid;
    static int count;
    if (FlagStop == 0)
        count = 0;
    if ((FlagStop == 1) && (pData->encoder_count_l < 10)) /* 此时停止且左轮不动 */
    {
        count += SYS_Abs(pData->encoder_count_l);
        if (count > 6 && count < 180) /* 普通模式 */
        {
            FlagFollow = 0;
            FlagAvoid = 0;
        }
        if (count > 180 && count < 360) /* 避障模式 */
        {
            FlagAvoid = 1;
            FlagFollow = 0;
        }
        if (count > 360 && count < 540) /* 跟随模式 */
        {
            FlagAvoid = 0;
            FlagFollow = 1;
        }
        if (count > 540)
            count = 0;
    }
}

#ifdef __cplusplus
}
#endif
