#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include <assert.h>

#include "led.h"
#include "main.h"

/* ANCHOR - 公共函数定义 */

/**
 * *****************************************************************************
 * @brief 什么也不做, 只是为了外设操作函数的统一化, 即使用外设模块之前一定要初始化外设
 * *****************************************************************************
 */
void LED_Init(void)
{
}

/**
 * *****************************************************************************
 * @brief 开启 LED 灯
 * *****************************************************************************
 */
void LED_On(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}

/**
 * *****************************************************************************
 * @brief 熄灭 LED 灯
 * *****************************************************************************
 */
void LED_Off(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

/**
 * *****************************************************************************
 * @brief 切换 LED 亮灭状态
 * *****************************************************************************
 */
void LED_Toggle(void)
{
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

/**
 * *****************************************************************************
 * @brief LED 闪烁
 * @param [in] time 闪烁间隔时间 单位: ms
 * @param [in] count 闪烁次数
 * @note - time 最大 3s, 再长就没有什么意义
 * *****************************************************************************
 */
void LED_Flash(u16 time, u8 count)
{
    /* 1. 参数检查 ------------------------------------------------------------------ */
    assert(time > 0 && time < 3000);
    assert(count > 0);

    /* 2. 功能实现 ------------------------------------------------------------------ */
    while (count)
    {
        HAL_Delay(time);
        LED_Toggle();
        HAL_Delay(time);
        LED_Toggle();
        count--;
    }
}

#ifdef __cplusplus
}
#endif
