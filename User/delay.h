#ifndef DELAY_H
#define DELAY_H

/* ANCHOR - 头文件包含 */

#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 公共函数声明 */

void Systick_Delay_Init(void);
void Systick_Delay_Ms(u32 nms);
void Systick_Delay_Us(u32 nus);
void Systick_Delay_ISR(void);
u64  Systick_GetUs(void);
u64  Systick_GetTick(void);

#ifdef __cplusplus
}
#endif

#endif /* DELAY_H */
