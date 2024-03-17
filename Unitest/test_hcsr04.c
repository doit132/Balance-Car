#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "test_hcsr04.h"
#include "hcsr04.h"
#include "tim.h"

/* ANCHOR - 全局变量定义 */

extern Hcsr04Info_t g_hcsr04_info;

/* ANCHOR - 公共函数定义 */

void Test_HCSR04(void)
{
    HCSR04_Init(&htim3, TIM_CHANNEL_3, &g_hcsr04_info);
    while (1)
    {
        HCSR04_Start();
        Hcsr04_Get_Data(&g_hcsr04_info);
        printf("distance = %f\r\n", g_hcsr04_info.distance);
        HAL_Delay(200);
    }
}

#ifdef __cplusplus
}
#endif
