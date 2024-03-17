#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "key.h"
#include "main.h"
#include "soft_timer.h"

/* ANCHOR - 枚举 */

typedef enum _KEY_StatusList_TypeDef /* 按键状态 */
{
    KEY_Status_Idle = 0,         /* 空闲 */
    KEY_Status_Debounce,         /* 消抖 */
    KEY_Status_ConfirmPress,     /* 确认按下 */
    KEY_Status_ConfirmPressLong, /* 确认长按着 */
    KEY_Status_WaitAgain,        /* 等待再次按下 */
    KEY_Status_SecondPress,      /* 第二次按下 */
} KEY_StatusList_TypeDef;

typedef enum /* 按键动作, 按下, 释放 */
{
    KEY_Action_Press = 0,
    KEY_Action_Release
} KEY_Action_TypeDef;

typedef enum /* 按键引脚的电平 */
{
    KKEY_PinLevel_Low = 0, /* 低电平 */
    KEY_PinLevel_High      /* 高电平 */
} KEY_PinLevel_TypeDef;

/* ANCHOR - 私有函数声明 */

static KEY_PinLevel_TypeDef _KEY_ReadPin(void);
static void                 _KEY_GetAction_PressOrRelease(void);

/* ANCHOR - 结构体 */

/* 按键配置结构体 */
typedef struct _KEY_Configure_TypeDef
{
    uint16_t               KEY_Count;              /* 按键长按计数 */
    KEY_Action_TypeDef     KEY_Action;             /* 按键动作，按下或抬起 */
    KEY_StatusList_TypeDef KEY_Status;             /* 按键状态 */
    KEY_EventList_TypeDef  KEY_Event;              /* 按键事件 */
    KEY_PinLevel_TypeDef (*KEY_ReadPin_Fcn)(void); /* 读 IO 电平函数 */
} KEY_Configure_TypeDef;

/* ANCHOR - 全局变量 */

KEY_Configure_TypeDef KeyCfg = {
    0,                  /* 按键长按计数 */
    KEY_Action_Release, /* 按键动作，按下或者抬起 */
    KEY_Status_Idle,    /* 按键状态 */
    KEY_Event_Null,     /* 按键事件 */
    _KEY_ReadPin        /* 读 IO 电平函数 */
};

/* ANCHOR - 宏定义 */

#define KEY_LONG_PRESS_TIME  50 /* 20ms*50 = 1s */
#define KEY_WAIT_DOUBLE_TIME 10 /* 20ms*10 = 200ms */
#define KEY_PRESSED_LEVEL    0  /* 按键按下是电平为低 */

/* ANCHOR - 私有函数定义 */

/**
 * *****************************************************************************
 * @brief 读取按键引脚的电平
 * @return KEY_PinLevel_TypeDef
 * *****************************************************************************
 */
static KEY_PinLevel_TypeDef _KEY_ReadPin(void)
{
    return (KEY_PinLevel_TypeDef)HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);
}

/**
 * *****************************************************************************
 * @brief 根据实际按下按钮的电平去把它换算成虚拟的结果
 * *****************************************************************************
 */
static void _KEY_GetAction_PressOrRelease(void)
{
    if (KeyCfg.KEY_ReadPin_Fcn() == KEY_PRESSED_LEVEL)
    {
        KeyCfg.KEY_Action = KEY_Action_Press;
    }
    else
    {
        KeyCfg.KEY_Action = KEY_Action_Release;
    }
}

/* ANCHOR - 公共函数定义 */

/**
 * *****************************************************************************
 * @brief 按键扫描状态机
 * *****************************************************************************
 */
void KEY_ReadStateMachine(void)
{
    _KEY_GetAction_PressOrRelease();

    switch (KeyCfg.KEY_Status)
    {
        /* 状态: 没有按键按下 */
        case KEY_Status_Idle:
            if (KeyCfg.KEY_Action == KEY_Action_Press) /* 释放->按下, 下一步是消抖 */
            {
                KeyCfg.KEY_Status = KEY_Status_Debounce;
                KeyCfg.KEY_Event = KEY_Event_Null;
            }
            else
            {
                KeyCfg.KEY_Status = KEY_Status_Idle;
                KeyCfg.KEY_Event = KEY_Event_Null;
            }
            break;

        /* 状态：消抖 */
        case KEY_Status_Debounce:
            if (KeyCfg.KEY_Action == KEY_Action_Press)
            {
                KeyCfg.KEY_Status = KEY_Status_ConfirmPress;
                KeyCfg.KEY_Event = KEY_Event_Null;
            }
            else
            {
                KeyCfg.KEY_Status = KEY_Status_Idle;
                KeyCfg.KEY_Event = KEY_Event_Null;
            }
            break;

        /* 状态: 继续按下 */
        case KEY_Status_ConfirmPress:
            if ((KeyCfg.KEY_Action == KEY_Action_Press) && (KeyCfg.KEY_Count >= KEY_LONG_PRESS_TIME))
            {
                KeyCfg.KEY_Status = KEY_Status_ConfirmPressLong;
                KeyCfg.KEY_Event = KEY_Event_Null;
                KeyCfg.KEY_Count = 0;
            }
            else if ((KeyCfg.KEY_Action == KEY_Action_Press) && (KeyCfg.KEY_Count < KEY_LONG_PRESS_TIME))
            {
                KeyCfg.KEY_Count++;
                KeyCfg.KEY_Status = KEY_Status_ConfirmPress;
                KeyCfg.KEY_Event = KEY_Event_Null;
            }
            else
            {
                KeyCfg.KEY_Count = 0;
                KeyCfg.KEY_Status = KEY_Status_WaitAgain; /* 短击后释放 */
                KeyCfg.KEY_Event = KEY_Event_Null;
            }
            break;

        /* 状态: 一直长按着 */
        case KEY_Status_ConfirmPressLong:
            if (KeyCfg.KEY_Action == KEY_Action_Press)
            {
                /* 一直等待其放开 */
                KeyCfg.KEY_Status = KEY_Status_ConfirmPressLong;
                KeyCfg.KEY_Event = KEY_Event_Null;
                KeyCfg.KEY_Count = 0;
            }
            else
            {
                KeyCfg.KEY_Status = KEY_Status_Idle;
                KeyCfg.KEY_Event = KEY_Event_LongPress;
                KeyCfg.KEY_Count = 0;
            }
            break;

        /* 状态: 等待是否再次按下 */
        case KEY_Status_WaitAgain:
            if ((KeyCfg.KEY_Action != KEY_Action_Press) && (KeyCfg.KEY_Count >= KEY_WAIT_DOUBLE_TIME))
            {
                /* 第一次短按, 且释放时间大于 KEY_WAIT_DOUBLE_TIME */
                KeyCfg.KEY_Count = 0;
                KeyCfg.KEY_Status = KEY_Status_Idle;
                KeyCfg.KEY_Event = KEY_Event_SingleClick; /* 响应单击 */
            }
            else if ((KeyCfg.KEY_Action != KEY_Action_Press) && (KeyCfg.KEY_Count < KEY_WAIT_DOUBLE_TIME))
            {
                /* 第一次短按, 且释放时间还没到 KEY_WAIT_DOUBLE_TIME */
                KeyCfg.KEY_Count++;
                KeyCfg.KEY_Status = KEY_Status_WaitAgain; /* 继续等待 */
                KeyCfg.KEY_Event = KEY_Event_Null;
            }
            else /* 第一次短按, 且还没到 KEY_WAIT_DOUBLE_TIME 第二次被按下 */
            {
                KeyCfg.KEY_Count = 0;
                KeyCfg.KEY_Status = KEY_Status_SecondPress; /* 第二次按下 */
                KeyCfg.KEY_Event = KEY_Event_Null;
            }
            break;
        case KEY_Status_SecondPress:
            if ((KeyCfg.KEY_Action == KEY_Action_Press) && (KeyCfg.KEY_Count >= KEY_LONG_PRESS_TIME))
            {
                KeyCfg.KEY_Status = KEY_Status_ConfirmPressLong; /* 第二次按的时间大于 KEY_LONG_PRESS_TIME */
                KeyCfg.KEY_Event = KEY_Event_SingleClick;        /* 先响应单击 */
                KeyCfg.KEY_Count = 0;
            }
            else if ((KeyCfg.KEY_Action == KEY_Action_Press) && (KeyCfg.KEY_Count < KEY_LONG_PRESS_TIME))
            {
                KeyCfg.KEY_Count++;
                KeyCfg.KEY_Status = KEY_Status_SecondPress;
                KeyCfg.KEY_Event = KEY_Event_Null;
            }
            else
            {
                /* 第二次按下后在 KEY_LONG_PRESS_TIME 内释放 */
                KeyCfg.KEY_Count = 0;
                KeyCfg.KEY_Status = KEY_Status_Idle;
                KeyCfg.KEY_Event = KEY_Event_DoubleClick; /* 响应双击 */
            }
            break;
        default:
            break;
    }
}

/**
 * *****************************************************************************
 * @brief 启动一个软件定时器, 周期性触发中断, 用来按键扫描
 * *****************************************************************************
 */
void KEY_Init(void)
{
    // SoftTimer_Start(1, 20, SOFT_TIMER_MODE_PERIODIC, "KEY");
}

/**
 * *****************************************************************************
 * @brief 主循环作为时基, 进行按键消抖和扫描 (while(1) 作为时基)
 * @return 按键键值编码
 * @note - 0: 无按键, 1: 短按, 2: 长按, 3: 双击
 * @note - WT: While(1) as Time base
 * *****************************************************************************
 */
u8 KEY_Scan_WT(void)
{
    KEY_ReadStateMachine();
    return KeyCfg.KEY_Event;
}

/**
 * *****************************************************************************
 * @brief 使用软件定时器作为时基, 每 2ms 执行一次 
 * @return 按键键值编码
 * @note - 0: 无按键, 1: 单击, 2: 双击, 4: 长按
 * @note - 检测按键并没有在中断处理程序中执行, 而是在主循环函数中执行
 * *****************************************************************************
 */
u8 KEY_Scan_ST(void)
{
    if (SoftTimer_Check(1))
    {
        KEY_ReadStateMachine();
        return KeyCfg.KEY_Event;
    }
    return 0;
}

#ifdef __cplusplus
}
#endif
