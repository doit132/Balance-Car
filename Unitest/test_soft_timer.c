#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "test_soft_timer.h"
#include "unity.h"
#include "soft_timer.h"
#include "led.h"

/* ANCHOR - 公共函数定义 */

void Test_SoftTimer_Led(void)
{
    SoftTimer_Init();
    SoftTimer_Start(0, 1000, SOFT_TIMER_MODE_PERIODIC, "LED");
    while (1)
    {
        if (SoftTimer_Check(0) == 1)
        {
            LED_Toggle();
        }
    }
}

int Test_SoftTimer(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_SoftTimer_Led);
    return UNITY_END();
}

#ifdef __cplusplus
}
#endif
