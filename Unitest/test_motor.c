#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "test_motor.h"
#include "motor.h"

/* ANCHOR - 公共函数定义 */

void Test_Motor(void)
{
    extern Motor_Data_t g_motor_data;
    Motor_Init(&g_motor_data);
    while (1)
    {
        // Motor_Set_Pwm(300, 300);
    }
}

#ifdef __cplusplus
}
#endif
