/* ANCHOR - 头文件包含 */

#include "task.h"
#include "bsp.h"
#include "adc.h"
#include "show.h"
#include "control.h"
#include "motor.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 公共函数定义 */

/**
 * *****************************************************************************
 * @brief 5ms 执行一次该任务
 * *****************************************************************************
 */
void Task_Data_Collect(void)
{
    g_key_num = KEY_Scan_WT();
    HCSR04_Start();
    Hcsr04_Get_Data(&g_hcsr04_info);
    Encoder_Get_Data(&g_encoder_data);
    Motor_Get_Velocity_From_Encoder(&g_motor_data, &g_encoder_data);
    g_voltage = ADC_Get_Battery_Volt();
    MPU6050_DMP_Get_Data(&g_pitch, &g_roll, &g_yaw);
}

void Task_Data_Show(void)
{
    OLED_Show();
}

/**
 * *****************************************************************************
 * @brief 按键启动
 * *****************************************************************************
 */
void Task_Key_Control(void)
{
    if (g_key_num == 1)
    {
        if (g_flag_stop == 1) /* 当前是停止状态 */
        {
            g_flag_stop = 0;
        }
        else /* 当前是启动状态 */
        {
            g_flag_stop = 1;
            Motor_TurnOff();
        }
    }
}

/**
 * *****************************************************************************
 * @brief 选中小车右轮, 改变小车的运动模式
 * *****************************************************************************
 */
void Task_Encoder_Control(void)
{
    extern u8  g_flag_stop, g_flag_follow, g_flag_avoid;
    static int count;
    if (g_flag_stop == 0)
    {
        count = 0;
    }
    if (g_flag_stop == 1 && g_encoder_data.count_l == 0) /* 此时停止 */
    {
        count += SYS_Abs(g_encoder_data.count_r);
        if (count > 6 && count < 400) /* 普通模式 */
        {
            g_flag_follow = 0;
            g_flag_avoid = 0;
        }
        if (count > 400 && count < 800) /* 避障模式 */
        {
            g_flag_avoid = 1;
            g_flag_follow = 0;
        }
        if (count > 800 && count < 1200) /* 跟随模式 */
        {
            g_flag_avoid = 0;
            g_flag_follow = 1;
        }
        if (count > 1200)
            count = 0;
    }
}

void Task_Pid_Control(void)
{
    int balance_pwm, velocity_pwm, pid_pwm;

    /* 直立环 PID 控制 */
    MPU6050_DMP_Get_Data(&g_pitch, &g_roll, &g_yaw);
    balance_pwm = Control_Balance_PD(g_pitch, 0, g_gyro_x);

    /* 速度环 PID 控制 */
    velocity_pwm = Control_Velocity_PI(&g_encoder_data);

    pid_pwm = balance_pwm + velocity_pwm;
    pid_pwm = PWM_Limit(pid_pwm, 6900, -6900);

    if (Control_Check_Pick_Up(&g_encoder_data)) /* 检查是否小车被拿起 */
        g_flag_stop = 1;                        /* 如果被拿起就关闭电机 */
    // if (Control_Check_Put_Down(&g_encoder_data)) /* 检查是否小车被放下 */
    //     g_flag_stop = 0;                         /* 如果被放下就启动电机 */
    if (Check_Exception() == 0)          /* 如果不存在异常 */
        Motor_Set_Pwm(pid_pwm, pid_pwm); /* 赋值给 PWM 寄存器 */
}

#ifdef __cplusplus
}
#endif
