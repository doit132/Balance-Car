#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "test_adc.h"
#include "adc.h"

/* ANCHOR - 公共函数定义 */

void Test_ADC_Voltage(void)
{
    extern float Voltage;
    while (1)
    {
        Voltage = ADC_Get_Battery_Volt();
        printf("voltage = %f\r\n", Voltage);
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
