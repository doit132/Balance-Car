#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "sys.h"

/* ANCHOR - 公共函数定义 */

void LED_Init(void);
void LED_Flash(u16 time, u8 count);
void LED_Toggle(void);

#ifdef __cplusplus
}
#endif
