#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include <string.h>

#include "soft_timer.h"

/* ANCHOR - 宏定义 */

/* ANCHOR - 私有全局变量定义 */

/* 定于软件定时器结构体变量 */
static soft_timer_t _soft_timer[TMR_COUNT];

/* ANCHOR - 私有函数定义 */

static void SoftTimer_Dec(soft_timer_t* tmr)
{
    if (tmr->Count > 0)
    {
        /* 如果定时器变量减到 1 则设置定时器到达标志 */
        if (--tmr->Count == 0)
        {
            tmr->Flag = 1; /* Flag = 1 在检查定时器时间中会用到 */
        }
    }
}

/* ANCHOR - 公共函数定义 */

void SoftTimer_Init(void)
{
    u8 i;
    /* 清零所有的软件定时器 */
    for (i = 0; i < TMR_COUNT; i++)
    {
        _soft_timer[i].Count = 0;
        strcpy(_soft_timer[i].Name, "soft_timer");
        _soft_timer[i].PreLoad = 0;
        _soft_timer[i].Flag = 0;
        _soft_timer[i].Mode = SOFT_TIMER_MODE_ONCE; /* 缺省是 1 次性工作模式 */
    }
    SysTick_Config(SystemCoreClock / 1000); /* SystemCoreClock / 1000 是重装载寄存器的值 LOAD 1ms */
}

/**
 * *****************************************************************************
 * @brief 启动软件定时器
 * @param [in] id 启动的软件定时器序号 0~TMR_COUNT - 1
 * @param [in] period 多少 ms
 * @param [in] mode 软件定时器工作模式
 * @param [in] name 软件定时器名称
 * *****************************************************************************
 */
void SoftTimer_Start(u8 id, u32 period, soft_timer_mode_t mode, const char* name)
{
    /* 防御式编程, 检测参数合法性 */
    assert(id < TMR_COUNT);
    assert(period > 0);
    assert(mode < SOFT_TIMER_MODE_MAX);
    assert(name != NULL && strlen(name) < TMR_NAME_LENGTH);

    __disable_irq();                  /* 关中断 */
    _soft_timer[id].Count = period;   /* 实时计数器初值 */
    _soft_timer[id].PreLoad = period; /* 计数器自动重装值, 仅自动模式起作用 */
    _soft_timer[id].Flag = 0;         /* 定时时间到标志 */
    _soft_timer[id].Mode = mode;      /* 工作模式 */
    strcpy((char*)_soft_timer[id].Name, (char*)name);
    __enable_irq(); /* 开中断 */
}

/**
 * *****************************************************************************
 * @brief 停止软件定时器
 * @param [in] id 要停止的软件定时器序号 0~TMR_COUNT - 1
 * *****************************************************************************
 */
void SoftTimer_Stop(u8 id)
{
    /* 防御式编程, 检测参数合法性 */
    assert(id < TMR_COUNT);

    __disable_irq();                             /* 关中断 */
    _soft_timer[id].Count = 0;                   /* 实时计数器初值 */
    _soft_timer[id].PreLoad = 0;                 /* 计数器自动重装值, 仅自动模式起作用 */
    _soft_timer[id].Flag = 0;                    /* 定时时间到标志 */
    _soft_timer[id].Mode = SOFT_TIMER_MODE_ONCE; /* 工作模式 */
    strcpy((char*)_soft_timer[id].Name, "soft_timer");
    __enable_irq(); /* 开中断 */
}

/**
 * *****************************************************************************
 * @brief 检测软件定时器是否定时完成
 * @param [in] id 要检测的软件定时器序号 0~TMR_COUNT - 1
 * @return 
 * *****************************************************************************
 */
u8 SoftTimer_Check(u8 id)
{
    /* 防御式编程, 检测参数合法性 */
    assert(id < TMR_COUNT);

    /* 判断时间到标志值 Flag 是否置位, 如果置位表示时间已经到, 如果为 0, 表示时间还没有到 */
    if (_soft_timer[id].Flag == 1)
    {
        _soft_timer[id].Flag = 0;
        _soft_timer[id].Count = 0;
        if (_soft_timer[id].Mode == SOFT_TIMER_MODE_PERIODIC)
        {
            _soft_timer[id].Count = _soft_timer[id].PreLoad; /* 定时时间到, 重新装载计数器初值 */
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * *****************************************************************************
 * @brief SysTick 中断处理程序, 被 stm32f1xx_it.c 的 SysTick_Handler 调用
 * *****************************************************************************
 */
void SysTick_ISR(void)
{
    u8 i;
    /* 每隔 1ms, 对软件定时器的计数器进行减一操作 */
    for (i = 0; i < TMR_COUNT; i++)
    {
        SoftTimer_Dec(&_soft_timer[i]);
    }
}

#ifdef __cplusplus
}
#endif
