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

    pData->count_l = 0;
    pData->count_r = 0;
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

    pData->count_l = Read_Encoder(2);
    pData->count_r = Read_Encoder(4);
}

#ifdef __cplusplus
}
#endif
