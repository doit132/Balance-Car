
/* ANCHOR - 头文件包含 */

#include "test_adc.h"
#include "adc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 公共函数定义 */

void Test_ADC_Voltage(void)
{
    extern float g_voltage;
    while (1)
    {
        g_voltage = ADC_Get_Battery_Volt();
        printf("voltage = %f\r\n", g_voltage);
        HAL_Delay(100);
    }
}

void Test_ADC(void)
{
    Test_ADC_Voltage();
}

#ifdef __cplusplus
}
#endif
