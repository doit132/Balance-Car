#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "test_oled.h"
#include "oled.h"

/* ANCHOR - 公共函数定义 */

void Test_OLED(void)
{
    OLED_Init();
    while (1)
    {
        OLED_ShowString(0, 0, "const char *p");
        OLED_Refresh_Gram();
    }
}
#ifdef __cplusplus
}
#endif
