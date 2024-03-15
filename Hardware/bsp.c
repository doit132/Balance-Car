#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "bsp.h"

/* ANCHOR - 公共函数定义 */

void BSP_Init(void)
{
    extern Hcsr04Info_t   Hcsr04Info;
    extern Encoder_Data_t Encoder_Data;
    extern Motor_Data_t   Motor_Data;
    Systick_Delay_Init();
    LED_Init();                                      /* LED 模块初始化 */
    KEY_Init();                                      /* 按键模块初始化 */
    Motor_Init(&Motor_Data);                         /* 电机模块初始化 */
    HCSR04_Init(&htim3, TIM_CHANNEL_3, &Hcsr04Info); /* 超声波模块初始化 */
    Encoder_Init(&Encoder_Data);
    OLED_Init(); /* OLED 模块初始化 */
}

#ifdef __cplusplus
}
#endif
