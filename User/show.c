/* ANCHOR - 头文件包含 */

#include "show.h"
#include "bsp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 公共函数定义 */

void OLED_Show(void)
{
    /* 第一行: 显示小车运行模式 */
    if (g_way_angle == 1)
        OLED_ShowString(0, 0, "DMP");
    else if (g_way_angle == 2)
        OLED_ShowString(0, 0, "Kalman");
    else if (g_way_angle == 3)
        OLED_ShowString(0, 0, "C F");

    if (g_flag_follow == 1)
        OLED_ShowString(70, 0, "Follow");
    else if (g_flag_avoid == 1)
        OLED_ShowString(70, 0, "Avoid ");
    else
        OLED_ShowString(70, 0, "Normal");

    /* 第二行: 显示角度 */
    OLED_ShowString(0, 10, "Angle");
    if (g_pitch < 0)
        OLED_ShowString(48, 10, "-");
    if (g_pitch >= 0)
        OLED_ShowString(48, 10, "+");
    OLED_ShowNumber(56, 10, SYS_Abs((int)g_pitch), 3, 12);

    /* 第三行: 显示角速度和超声波测距的距离 */
    OLED_ShowString(0, 20, "Gyrox");
    if (g_gyro_x < 0)
        OLED_ShowString(42, 20, "-");
    if (g_gyro_y >= 0)
        OLED_ShowString(42, 20, "+");
    OLED_ShowNumber(50, 20, SYS_Abs((int)g_gyro_y), 4, 12);

    OLED_ShowNumber(82, 20, (u16)g_hcsr04_info.distance * 10, 5, 12);
    OLED_ShowString(114, 20, "mm");

    /* 第四行: 显示左编码器读数与左电机速度 (mm/s) */
    OLED_ShowString(0, 30, "L");
    if (g_encoder_data.count_l < 0)
        OLED_ShowString(16, 30, "-"), OLED_ShowNumber(26, 30, SYS_Abs((int)g_encoder_data.count_l), 4, 12);
    if (g_encoder_data.count_l >= 0)
        OLED_ShowString(16, 30, "+"), OLED_ShowNumber(26, 30, SYS_Abs((int)g_encoder_data.count_l), 4, 12);

    if (g_motor_data.velocity_left < 0)
        OLED_ShowString(60, 30, "-");
    if (g_motor_data.velocity_left >= 0)
        OLED_ShowString(60, 30, "+");
    OLED_ShowNumber(68, 30, SYS_Abs((int)g_motor_data.velocity_left), 4, 12);
    OLED_ShowString(96, 30, "mm/s");

    /* 第五行: 显示右编码器读数与右电机速度 (mm/s) */
    OLED_ShowString(0, 40, "R");
    if (g_encoder_data.count_r < 0)
    {
        OLED_ShowString(16, 40, "-");
        OLED_ShowNumber(26, 40, SYS_Abs((int)g_encoder_data.count_r), 4, 12);
    }
    if (g_encoder_data.count_r >= 0)
    {
        OLED_ShowString(16, 40, "+");
        OLED_ShowNumber(26, 40, SYS_Abs((int)g_encoder_data.count_r), 4, 12);
    }

    if (g_motor_data.velocity_right < 0)
        OLED_ShowString(60, 40, "-");
    if (g_motor_data.velocity_right >= 0)
        OLED_ShowString(60, 40, "+");
    OLED_ShowNumber(68, 40, SYS_Abs((int)g_motor_data.velocity_right), 4, 12);
    OLED_ShowString(96, 40, "mm/s");

    /* 第六行: 显示电压与电机开关 */
    int volt = g_voltage * 100; /* 将电压扩大 100 倍, 转换为整数 */
    OLED_ShowString(0, 50, "V");
    OLED_ShowString(30, 50, ".");
    OLED_ShowString(64, 50, "V");
    OLED_ShowNumber(19, 50, volt / 100, 2, 12);
    OLED_ShowNumber(42, 50, volt / 10 % 10, 1, 12);
    OLED_ShowNumber(50, 50, volt % 10, 1, 12);
    if (g_flag_stop)
        OLED_ShowString(95, 50, "OFF");
    if (!g_flag_stop)
        OLED_ShowString(95, 50, "ON ");

    /* 刷新显示 */
    OLED_Refresh_Gram();
}

#ifdef __cplusplus
}
#endif
