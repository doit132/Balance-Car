#ifndef SOFT_TIMER_H
#define SOFT_TIMER_H

/* ANCHOR - 头文件包含 */

#include "sys.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 宏定义 */

/* 配置项 */
#define TMR_COUNT       4  /* 软件定时器的个数 (定时器 ID 范围 0 - 3) */
#define TMR_NAME_LENGTH 20 /* 软件定时器名长度 */

/* ANCHOR - 枚举定义 */

typedef enum soft_timer_mode
{
    SOFT_TIMER_MODE_ONCE = 0,
    SOFT_TIMER_MODE_PERIODIC,
    SOFT_TIMER_MODE_MAX
} soft_timer_mode_t;

/* ANCHOR - 结构体声明 */

/* 定时器结构体, 成员变量必须是 volatile, 否则 C 编译器优化时可能有问题 */
typedef struct
{
    volatile u8  Mode;                  /* 计数器模式, 1 次性 */
    volatile u8  Flag;                  /* 定时到达标志  */
    volatile u32 Count;                 /* 计数器 */
    volatile u32 PreLoad;               /* 计数器预装值 */
    char         Name[TMR_NAME_LENGTH]; /* 软件定时器名 */
} soft_timer_t;

/* ANCHOR - 公共函数声明 */

void SoftTimer_Init(void);
u8   SoftTimer_Check(u8 id);
void SoftTimer_Start(u8 id, u32 period, soft_timer_mode_t mode, const char* name);
void SoftTimer_Stop(u8 id);
/* NOTE - 中断处理函数 */

void SysTick_ISR(void);

#ifdef __cplusplus
}
#endif

#endif /* SOFT_TIMER_H */
