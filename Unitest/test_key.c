#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "unity.h"
#include "key.h"
#include "led.h"
#include "main.h"

/* ANCHOR - 公共函数定义 */

void Test_Key_Click(void)
{
    u8 key_num;
    while (1)
    {
        /* 1. 使用 while(1) 作为时基 ------------------------------------------------------ */
        // HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
        // key_num = KEY_Scan_WT();
        // if (key_num == 1) /* 短击 */
        // {
        //     LED_Toggle();
        // }

        // if (key_num == 2) /* 长击 */
        // {
        //     LED_Flash(500, 2);
        // }

        /* 2. 使用软件定时器中断作为时基 --------------------------------------------------------- */
        HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
        key_num = KEY_Scan_ST();
        if (key_num == 1)
        {
            LED_Toggle();
        }

        if (key_num == 2) /* 长击 */
        {
            LED_Flash(500, 2);
        }
    }
}

int Test_Key(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_Key_Click);
    return UNITY_END();
}

#ifdef __cplusplus
}
#endif
