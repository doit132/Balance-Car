#ifndef KEY_H
#define KEY_H

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "sys.h"

/* ANCHOR - 枚举 */

typedef enum _KEY_EventList_TypeDef /* 按键事件 */
{
    KEY_Event_Null = 0x00,        /* 无事件 */
    KEY_Event_SingleClick = 0x01, /* 单击 */
    KEY_Event_DoubleClick = 0x02, /* 双击 */
    KEY_Event_LongPress = 0x04    /* 长按 */
} KEY_EventList_TypeDef;

/* ANCHOR - 公共函数声明 */

void KEY_Init(void);
u8   KEY_Scan_WT(void);
u8   KEY_Scan_ST(void);

#ifdef __cplusplus
}
#endif

#endif /* KEY_H */
