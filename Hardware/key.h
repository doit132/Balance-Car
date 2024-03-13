#ifndef KEY_H
#define KEY_H

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "sys.h"

/* ANCHOR - 公共函数声明 */

void KEY_Init(void);
u8   KEY_Scan_WT(void);
u8   KEY_Scan_EXTI_Timer(void);
u8   KEY_Scan_ST(void);
#ifdef __cplusplus
}
#endif

#endif /* KEY_H */
