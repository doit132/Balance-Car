#ifndef BSP_H
#define BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "key.h"
#include "led.h"
#include "hcsr04.h"
#include "tim.h"
#include "oled.h"

/* ANCHOR - 公共函数声明 */

void BSP_Init(void);
#ifdef __cplusplus
}
#endif

#endif /* BSP_H */
