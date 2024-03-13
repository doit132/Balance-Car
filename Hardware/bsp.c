#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "bsp.h"

/* ANCHOR - 公共函数定义 */

void BSP_Init(void)
{
    LED_Init();
    KEY_Init();
}

#ifdef __cplusplus
}
#endif
