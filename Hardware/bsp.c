#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "bsp.h"

/* ANCHOR - 公共函数定义 */

void BSP_Init(void)
{
    extern Hcsr04Info_t Hcsr04Info;
    LED_Init();                                      /* LED 模块初始化 */
    KEY_Init();                                      /* 按键模块初始化 */
    HCSR04_Init(&htim3, TIM_CHANNEL_3, &Hcsr04Info); /* 超声波模块初始化 */
    OLED_Init();                                     /* OLED 模块初始化 */
}

#ifdef __cplusplus
}
#endif
