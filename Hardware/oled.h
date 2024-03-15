#ifndef OLED_H
#define OLED_H

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */
#include "sys.h"
#include "main.h"

/* ANCHOR - 宏定义 */

/* OLED IO 口操作宏定义 */
/* RST 引脚置位和复位 */
#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET); //RST
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET);   //RST
/* DC 引脚置位和复位 */
#define OLED_DC_Clr() HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET); //DC
#define OLED_DC_Set() HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);   //DC
/* SCLK 引脚置位和复位 */
#define OLED_SCLK_Clr() HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_RESET); //SCL
#define OLED_SCLK_Set() HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_SET);   //SCL
/* SDA 引脚置位和复位 */
#define OLED_SDA_Clr() HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_RESET); //SDA
#define OLED_SDA_Set() HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_SET);   //SDA

/* 表示写入是命令还是数据的标志 */
#define OLED_CMD  0 /* 写命令 */
#define OLED_DATA 1 /* 写数据 */

/* 中文字体的显示宽度和高度 */
#define CNSizeWidth  16
#define CNSizeHeight 16

/* ANCHOR - 公共函数声明 */

/* OLED 控制用函数 */
void OLED_WR_Byte(u8 dat, u8 cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x, u8 y, u8 t);
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size, u8 mode);
void OLED_ShowNumber(u8 x, u8 y, u32 num, u8 len, u8 size);
void OLED_ShowString(u8 x, u8 y, const char* p);
void OLED_ShowCHinese(u8 x, u8 y, u8 no, u8 font_width, u8 font_height);
void OLED_Set_Pos(u8 x, u8 y);

#ifdef __cplusplus
}
#endif

#endif /* OLED_H */
