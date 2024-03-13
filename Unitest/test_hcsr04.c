#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "test_hcsr04.h"
#include "hcsr04.h"
#include "tim.h"

/* ANCHOR - 全局变量定义 */

Hcsr04Info_t Hcsr04Info;

/* ANCHOR - 公共函数定义 */

void Test_HCSR04(void)
{
    HCSR04_Init(&htim3, TIM_CHANNEL_3, &Hcsr04Info);
    while (1)
    {
        HCSR04_Start();
        Hcsr04_Get_Data(&Hcsr04Info);
        printf("distance = %f\r\n", Hcsr04Info.distance);
        HAL_Delay(200);
    }
}

#ifdef __cplusplus
}
#endif
