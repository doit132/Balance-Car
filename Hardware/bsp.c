
/* ANCHOR - 头文件包含 */

#include "bsp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 公共函数定义 */

void BSP_Init(void)
{
    extern Hcsr04Info_t   g_hcsr04_info;
    extern Encoder_Data_t g_encoder_data;
    extern Motor_Data_t   g_motor_data;
    Systick_Delay_Init();
    LED_Init();                                         /* LED 模块初始化 */
    KEY_Init();                                         /* 按键模块初始化 */
    Motor_Init(&g_motor_data);                          /* 电机模块初始化 */
    HCSR04_Init(&htim3, TIM_CHANNEL_3, &g_hcsr04_info); /* 超声波模块初始化 */
    Encoder_Init(&g_encoder_data);
    MPU6050_DMP_Init();
    OLED_Init(); /* OLED 模块初始化 */
}

#ifdef __cplusplus
}
#endif
