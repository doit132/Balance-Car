#ifndef SYS_H
#define SYS_H

/* ANCHOR - Typedef */

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

/* ANCHOR - 头文件包含 */

#include <stdio.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 公共函数声明 */

int SYS_Abs(int a);

#ifdef __cplusplus
}
#endif

#endif /* SYS_H */
