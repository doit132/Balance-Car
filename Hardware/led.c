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

void LED_On(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}

void LED_Off(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

/**
 * *****************************************************************************
 * @brief LED 闪烁
 * @param [in] time 闪烁间隔时间
 * @note - time 最大 3s, 再长就没有什么意义
 * *****************************************************************************
 */
void LED_Flash(u16 time)
{
    /* 1. 参数检查 ------------------------------------------------------------------ */
    assert(time > 0 && time < 3000);

    /* 2. 功能实现 ------------------------------------------------------------------ */
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    HAL_Delay(time);
}

/* ANCHOR - LED 测试代码 */

static void Test_Led_Flash(void)
{
    LED_Init();
    while (1)
    {
        /* 1. 测试 LED 闪烁 ------------------------------------------------------------- */
        LED_Flash(1000);

        /* 2. 测试 led 亮 -------------------------------------------------------------- */
        // LED_On();

        /* 3. 测试 led 灭 -------------------------------------------------------------- */
        // LED_Off();
    }
}

void Test_LED(void)
{
    Test_Led_Flash();
}

#ifdef __cplusplus
}
#endif
