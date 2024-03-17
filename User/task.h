#ifndef TASK_H
#define TASK_H

/* ANCHOR - 头文件包含 */

#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 公共函数声明 */

void Task_Data_Collect(void);
void Task_Data_Show(void);
void Task_Key_Control(void);
void Task_Pid_Control(void);
void Task_Encoder_Control(void);

#ifdef __cplusplus
}
#endif

#endif /* TASK_H */
