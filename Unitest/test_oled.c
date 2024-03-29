
/* ANCHOR - 头文件包含 */

#include "test_oled.h"
#include "oled.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 公共函数定义 */

void Test_OLED_ShowString(void)
{
    OLED_Init();
    while (1)
    {
        OLED_ShowString(0, 0, "const char *p");
        OLED_Refresh_Gram();
    }
}

void Test_OLED(void)
{
    Test_OLED_ShowString();
}
#ifdef __cplusplus
}
#endif
