#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "bsp.h"
#include "key.h"
#include "led.h"
#include "hcsr04.h"
#include "tim.h"

/* ANCHOR - 公共函数定义 */

void BSP_Init(void)
{
    extern Hcsr04Info_t Hcsr04Info;
    LED_Init();
    KEY_Init();
    HCSR04_Init(&htim3, TIM_CHANNEL_3, &Hcsr04Info);
}

#ifdef __cplusplus
}
#endif
