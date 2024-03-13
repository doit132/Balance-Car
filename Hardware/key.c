#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "key.h"
#include "main.h"
#include "soft_timer.h"

/* ANCHOR - 枚举 */

/* ANCHOR - 宏定义 */

#define KEY HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin)

#define KEY_PRESSED 0u
#define KEY_RELEASE 1u

#define KEY_FILTER_TIME_WT 1000   /* 按键消抖延时时间 */
#define KEY_LONG_TIME_WT   300000 /* 按键长击时间, 需要不断的尝试, 才能确定长击的时间 */

#define KEY_FILTER_TIME_ST 5   /* 软件定时器 2ms 一次, 按键消抖延时时间 2*5=10ms */
#define KEY_LONG_TIME_ST   300 /* 软件定时器 2ms 一次, 按键长击时间, 2*300=600ms */

/* ANCHOR - 公共函数定义 */

/**
 * *****************************************************************************
 * @brief 什么也不做, 只是为了外设操作函数的统一化, 即使用外设模块之前一定要初始化外设
 * *****************************************************************************
 */
void KEY_Init(void)
{
    SoftTimer_Start(1, 2, SOFT_TIMER_MODE_PERIODIC, "KEY");
}

/**
 * *****************************************************************************
 * @brief 主循环作为时基, 进行按键消抖和扫描 (while(1) 作为时基)
 * @return 按键键值编码
 * @note - 0: 无按键, 1: 短按, 2: 长按
 * @note - WT: While(1) as Time base
 * *****************************************************************************
 */
u8 KEY_Scan_WT(void)
{
    static u8  key_lock_flag;          /* 按键自锁标志, 防止按键长按多次触发 */
    static u32 key_pressed_filter_cnt; /* 按键按下消抖计数器, 用来按键按下消抖延时 */
    static u32 key_release_filter_cnt; /* 按键释放消抖计数器, 用来按键释放消抖延时 */
    static u32 key_long_cnt;           /* 按键长击计数器, 用来判断长击时间 */
    static u8  key_short_flag;         /* 按键短击标志 */
    u8         key_num = 0;            /* 最终返回值, 按键键值编码 */

    if (KEY == KEY_RELEASE) /* 按键释放进入该条件分支 */
    {
        /**
         * 按键释放之前处于稳定的按下状态 (按下->释放)
         * 按键一直处于稳定的释放状态 (释放->释放)
         */

        if (key_lock_flag == 1 || key_short_flag == 1) /* 按键释放之前处于稳定的按下状态 (按下->释放) */
        {
            /**
             * 按键释放 (不稳定状态)
             *   1. 按键按下产生的抖动 
             *   2. 按键释放产生的抖动
             * 按键释放 (从释放不稳定状态->释放稳定状态)
             */
            /* 消除按键按下和释放产生的抖动 */
            key_long_cnt = 0;
            key_pressed_filter_cnt = 0;
            key_release_filter_cnt++;
            if (key_release_filter_cnt > KEY_FILTER_TIME_WT) /* 按键释放 (从释放不稳定状态->释放稳定状态) */
            {
                /**
                 * 1. 按键短击释放
                 * 2. 按键长击释放
                 */
                if (key_short_flag) /* 按键短击释放 */
                {
                    key_short_flag = 0;
                    key_num = 1;
                }
                else /* 按键长击释放 */
                {
                    key_long_cnt = 0;
                }
                key_lock_flag = 0;
            }
        }
        /* 按键一直处于稳定的释放状态 (释放->释放) */
        key_lock_flag = 0;
    }
    else if (key_lock_flag == 0 || key_short_flag == 1) /* 按键第一次按下, 进入该条件分支 */
    {
        /**
         * 按键按下之前处于稳定的释放状态 (释放->按下)
         *   按键按下 (不稳定状态)
         *     1. 按键按下产生的抖动
         *     2. 按键释放产生的抖动 (不可能发生, 因为之前的按键处于稳定按下状态, key_lock_flag = 1)
         *   按键按下 (稳定状态)
         *     1. 短击 
         *     2. 长按 
         * 按键一直处于稳定的按下状态 (按下->按下)
         */

        /* 消除按键按下产生的抖动 */
        key_pressed_filter_cnt++;
        if (key_pressed_filter_cnt > KEY_FILTER_TIME_WT) /* 表示按键真正的按下 (稳定状态) */
        {
            /**
             * 1. 短击
             * 2. 长按
             */
            key_long_cnt++;
            key_short_flag = 1;
            if (key_long_cnt > KEY_LONG_TIME_WT) /* 长击, 在按键按下状态进行判断 */
            {
                key_short_flag = 0; /* 清除短击标志 */
                key_long_cnt = 0;
                key_lock_flag = 1; /* 自锁标志位置 1, 防止按键多次触发 */
                key_num = 2;       /* 按键键值编码 = 2, 表示有按键长按 */
            }
        }
    }
    else /* 按键按下但不是第一次按下, 进入该条件分支 */
    {
        /**
         * 1. 按键释放产生抖动, 导致检测到的按键处于按下状态
         * 2. 按键长按
         */
        key_release_filter_cnt = 0;
    }

    return key_num;
}

/**
 * *****************************************************************************
 * @brief 使用软件定时器作为时基, 每 2ms 执行一次 
 * @return 按键键值编码
 * @note - 0: 无按键, 1: 短按, 2: 长按
 * @note - 检测按键并没有在中断处理程序中执行, 而是在主循环函数中执行
 * *****************************************************************************
 */
u8 KEY_Scan_ST(void)
{
    static u8  key_lock_flag;          /* 按键自锁标志, 防止按键长按多次触发 */
    static u32 key_pressed_filter_cnt; /* 按键按下消抖计数器, 用来按键按下消抖延时 */
    static u32 key_release_filter_cnt; /* 按键释放消抖计数器, 用来按键释放消抖延时 */
    static u32 key_long_cnt;           /* 按键长击计数器, 用来判断长击时间 */
    static u8  key_short_flag;         /* 按键短击标志 */
    u8         key_num = 0;            /* 最终返回值, 按键键值编码 */

    if (SoftTimer_Check(1)) /* 每 2ms 执行一次 */
    {
        if (KEY == KEY_RELEASE) /* 按键释放进入该条件分支 */
        {
            /**
             * 按键释放之前处于稳定的按下状态 (按下->释放)
             * 按键一直处于稳定的释放状态 (释放->释放)
             */

            if (key_lock_flag == 1 || key_short_flag == 1) /* 按键释放之前处于稳定的按下状态 (按下->释放) */
            {
                /**
                 * 按键释放 (不稳定状态)
                 *   1. 按键按下产生的抖动 
                 *   2. 按键释放产生的抖动
                 * 按键释放 (从释放不稳定状态->释放稳定状态)
                 */
                /* 消除按键按下和释放产生的抖动 */
                key_long_cnt = 0;
                key_pressed_filter_cnt = 0;
                key_release_filter_cnt++;
                if (key_release_filter_cnt > KEY_FILTER_TIME_ST) /* 按键释放 (从释放不稳定状态->释放稳定状态) */
                {
                    /**
                     * 1. 按键短击释放
                     * 2. 按键长击释放
                     */
                    if (key_short_flag) /* 按键短击释放 */
                    {
                        key_short_flag = 0;
                        key_num = 1;
                    }
                    else /* 按键长击释放 */
                    {
                        key_long_cnt = 0;
                    }
                    key_lock_flag = 0;
                }
            }
            /* 按键一直处于稳定的释放状态 (释放->释放) */
            key_lock_flag = 0;
        }
        else if (key_lock_flag == 0 || key_short_flag == 1) /* 按键第一次按下, 进入该条件分支 */
        {
            /**
             * 按键按下之前处于稳定的释放状态 (释放->按下)
             *   按键按下 (不稳定状态)
             *     1. 按键按下产生的抖动
             *     2. 按键释放产生的抖动 (不可能发生, 因为之前的按键处于稳定按下状态, key_lock_flag = 1)
             *   按键按下 (稳定状态)
             *     1. 短击 
             *     2. 长按 
             * 按键一直处于稳定的按下状态 (按下->按下)
             */

            /* 消除按键按下产生的抖动 */
            key_pressed_filter_cnt++;
            if (key_pressed_filter_cnt > KEY_FILTER_TIME_ST) /* 表示按键真正的按下 (稳定状态) */
            {
                /**
                 * 1. 短击
                 * 2. 长按
                 */
                key_long_cnt++;
                key_short_flag = 1;
                if (key_long_cnt > KEY_LONG_TIME_ST) /* 长击, 在按键按下状态进行判断 */
                {
                    key_short_flag = 0; /* 清除短击标志 */
                    key_long_cnt = 0;
                    key_lock_flag = 1; /* 自锁标志位置 1, 防止按键多次触发 */
                    key_num = 2;       /* 按键键值编码 = 2, 表示有按键长按 */
                }
            }
        }
        else /* 按键按下但不是第一次按下, 进入该条件分支 */
        {
            /**
             * 1. 按键释放产生抖动, 导致检测到的按键处于按下状态
             * 2. 按键长按
             */
            key_release_filter_cnt = 0;
        }
    }
    return key_num;
}

u8 KEY_Scan_EXTI_Timer(void)
{
    u8 key_num;
    key_num = 0;
    if (SoftTimer_Check(1))
    {
        if (KEY == KEY_PRESSED)
        {
            key_num = 1;
        }
    }
    return key_num;
}

/**
 * *****************************************************************************
 * @brief 按键中断回调函数 
 * *****************************************************************************
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == KEY_Pin)
    {
        SoftTimer_Start(1, 2, SOFT_TIMER_MODE_ONCE, "KEY");
    }
}

#ifdef __cplusplus
}
#endif
