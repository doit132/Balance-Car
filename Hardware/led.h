#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "sys.h"

/* ANCHOR - 公共函数定义 */

void LED_Init(void);
void LED_Flash(u16 time);

/* NOTE - 测试函数 */

void Test_LED(void);

#ifdef __cplusplus
}
#endif
