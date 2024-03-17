#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "stm32f1xx_hal.h"
#include "core_cm3.h"
#include "delay.h"

/* ANCHOR - 私有全局变量定义 */

static __IO u64 ulTicks;
static float    us_per_mini_tick;

/* ANCHOR - 公共函数定义 */

void Systick_Delay_Init(void)
{
    /*
        将 Systick 配置为 1ms 发生一次溢出事件
        us_per_mini_tick: 发生一次 Systick 计数消耗的时间
     */
    us_per_mini_tick = 1000.0 / ((SysTick->LOAD & 0x00ffffff) + 1); /* 发生一次 Systick 计数消耗的时间 */
}

/**
 * *****************************************************************************
 * @brief 系统滴答定时器中断的处理代码
 * @note - 此方法应当在 Systick_IRQHandler 中被调用
 * *****************************************************************************
 */
void Systick_Delay_ISR(void)
{
    ulTicks++;
}

/**
 * *****************************************************************************
 * @brief 获取当前时间, 以毫秒 (千分之一秒) 为单位
 * @return 当前时间, 单位为毫秒 (千分之一秒)
 * *****************************************************************************
 */
u64 Systick_GetTick(void)
{
    return ulTicks;
}

/**
 * *****************************************************************************
 * @brief 获取当前时间, 以微秒 (百万分之一秒) 为单位
 * @return 当前时间, 单位为微秒 (百万分之一秒)
 * *****************************************************************************
 */
u64 Systick_GetUs(void)
{
    u64 tick;
    u32 mini_tick;

    SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk; // 清除 COUNTFLAG

    tick = ulTicks;           // 读取毫秒值
    mini_tick = SysTick->VAL; // 读取 SYSTICK 的值

    while (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) /* 上次发生了 Systick 溢出事件 */
    {
        mini_tick = SysTick->VAL;
        tick = ulTicks;
    }

    // 换算成微秒
    tick *= 1000;                                                  // 毫秒部分乘以 1000
    tick += (u32)((SysTick->LOAD - mini_tick) * us_per_mini_tick); // 小数部分折算成微秒

    return tick;
}

/**
 * *****************************************************************************
 * @brief 微秒级延迟
 * @param [in] nus 延迟时长，单位微秒(百万分之一秒)
 * @note - 不允许在中断响应函数中调用此方法
 * *****************************************************************************
 */
void Systick_Delay_Us(u32 nus)
{
    uint64_t expiredTime = Systick_GetUs() + nus;

    while (Systick_GetUs() < expiredTime)
        ;
}

/**
 * *****************************************************************************
 * @brief 毫秒级延迟
 * @param [in] nms 延迟时长, 以毫秒为单位 (千分之一秒)
 * @note - 不允许在中断响应函数中调用此方法
 * *****************************************************************************
 */
void Systick_Delay_Ms(u32 nms)
{
    u64 expiredTime = ulTicks + nms;

    while (ulTicks < expiredTime)
    {
    }
}

#ifdef __cplusplus
}
#endif
