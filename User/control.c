/* ANCHOR - 头文件包含 */

#include "control.h"
#include "bsp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 公共函数定义 */

/**
 * *****************************************************************************
 * @brief 异常关闭电机
 * @return 1: 关闭电机(发生异常) 0: 未关闭电机(没有异常)
 * *****************************************************************************
 */
u8 Check_Exception(void)
{
    u8 temp;
    /* 
     * 电池电压低于 10V 关闭电机
     * 倾角大于 40 度关闭电机
     * Flag_Stop 置 1, 即单击控制关闭电机
     */
    if (g_pitch < -40 || g_pitch > 40 || 1 == g_flag_stop || g_voltage < 10)
    {
        temp = 1;
        Motor_TurnOff();
    }
    else
        temp = 0;
    return temp;
}

/**
 * *****************************************************************************
 * @brief 
 * @param [in] angle 小车倾斜的角度
 * @param [in] mechanical_balance 机械平衡角度 (机械中值)
 * @param [in] gyro 小车倾斜的角速度
 * @return 
 * @note - 直立环 PD 控制器
 * *****************************************************************************
 */
int Control_Balance_PD(float angle, float mechanical_balance, float gyro)
{
    float bias;        /* 角度误差 */
    int   balance_pwm; /* 直立环计算出来的电机控制 pwm */

    bias = angle - mechanical_balance;
    /* 求出平衡的角度中值和机械相关, 角度的微分就是角速度 */
    balance_pwm = g_balance_kp * bias + g_balance_kd * gyro;
    /* 计算平衡控制的电机 PWM */
    return balance_pwm;
}

/**
 * *****************************************************************************
 * @brief 速度控制 PWM	
 * @param [in] encoder_data 编码器数据
 * @return 速度控制 PWM
 * @note - 修改前进后退速度, 请修改 Target_Velocity, 比如, 改成 60 就比较慢了
 * *****************************************************************************
 */
int Control_Velocity_PI(Encoder_Data_t* encoder_data)
{
    static float velocity, encoder_err, Encoder_bias;
    static float Encoder_Integral; /* 存储误差的积分值 */

    /* 速度 PI 控制 ----------------------------------------------------------------- */
    /* 获取最新速度偏差 */
    encoder_err = g_encoder_data.count_l + g_encoder_data.count_r - 0;
    Encoder_bias *= 0.86;               /* 一阶低通滤波器 */
    Encoder_bias += encoder_err * 0.14; /* 一阶低通滤波器, 减缓速度变化 */
    Encoder_Integral += Encoder_bias;   /* 积分出位移 积分时间: 10ms */
    if (Encoder_Integral > 10000)
        Encoder_Integral = 10000; /* 积分限幅 */
    if (Encoder_Integral < -10000)
        Encoder_Integral = -10000;                                              /* 积分限幅 */
    velocity = g_velocity_kp * Encoder_bias + g_velocity_ki * Encoder_Integral; /* 速度控制 */
    if (Check_Exception() == 1)
        Encoder_Integral = 0; /* 电机关闭后清除积分 */
    return velocity;
}

/**
 * *****************************************************************************
 * @brief 限制 PWM 赋值
 * @param [in] IN 输入参数
 * @param [in] max 限幅最大值
 * @param [in] min 限幅最小值
 * @return 限幅后的值
 * *****************************************************************************
 */
int PWM_Limit(int IN, int max, int min)
{
    int OUT = IN;
    if (OUT > max)
        OUT = max;
    if (OUT < min)
        OUT = min;
    return OUT;
}

/**
 * *****************************************************************************
 * @brief 判断小车是否被拿起
 * @param [in] mpu6050_data MPU6050 数据
 * @param [in] encoder_data 编码器数据
 * @return 拿起返回 1, 未拿起返回 0
 * *****************************************************************************
 */
int Control_Check_Pick_Up(Encoder_Data_t* encoder_data)
{
    static u16 count0, count1, count2, flag;

    if (flag == 0) // 第一步
    {
        if (SYS_Abs(encoder_data->count_l) + SYS_Abs(encoder_data->count_r) < 30) // 条件 1, 小车接近静止
            count0++;
        else
            count0 = 0;

        if (count0 > 10) /* 100 ms */
        {
            flag = 1;
            count0 = 0;
        }
    }
    if (flag == 1) // 进入第二步
    {
        if (++count1 > 200) /* 2000 ms */
        {
            count1 = 0;
            flag = 0;
        } // 超时不再等待 2000ms, 返回第一步

        if (g_acc_z >= 1 && g_pitch > -10 && g_pitch < 10) // 条件2，小车是在0度附近被拿起
            flag = 2;
    }
    if (flag == 2)
    {
        if (SYS_Abs(encoder_data->count_l) + SYS_Abs(encoder_data->count_r) > 70 && g_flag_stop == 0)
        {
            if (++count2 > 100) /* 1000 ms */
            {
                flag = 0;
                count2 = 0;
                return 1; /* 检测到小车被拿起 */
            }
        }
    }

    return 0;
}

/**
 * *****************************************************************************
 * @brief 检测小车是否被放下
 * @param [in] encoder_data 编码器数据 
 * @return 放下返回 1, 未放下返回 0
 * *****************************************************************************
 */
int Control_Check_Put_Down(Encoder_Data_t* encoder_data)
{
    static u16 flag, count;
    if (g_flag_stop == 0) /* 防止误检 */
        return 0;
    if (flag == 0)
    {
        if (g_pitch > -10 && g_pitch < 10 && encoder_data->count_l == 0 &&
            encoder_data->count_r == 0) /* 条件 1, 小车是在 0 度附近的 */
        {
            flag = 1;
        }
    }
    if (flag == 1)
    {
        if (++count > 50) /* 超时不再等待 500ms */
        {
            count = 0;
            flag = 0;
        }
        if (encoder_data->count_l > 3 && encoder_data->count_r > 3 && encoder_data->count_l < 40 &&
            encoder_data->count_r < 40) /* 条件 2, 小车的轮胎在未上电的时候被人为转动 */
        {
            flag = 0;
            count = 0;
            return 1; /*  检测到小车被放下 */
        }
    }
    return 0;
}

/**
 * *****************************************************************************
 * @brief 通过旋转小车轮子, 选择小车运行模式
 * @param [in] encoder_data 编码器数据
 * *****************************************************************************
 */
void Car_Mode_Choose(Encoder_Data_t* encoder_data)
{
    static int count;
    if (g_flag_stop == 0)
        count = 0;
    if ((g_flag_stop == 1) && (encoder_data->count_l < 10)) /* 此时停止且左轮不动 */
    {
        count += SYS_Abs(encoder_data->count_r);
        if (count > 6 && count < 180) /* 普通模式 */
        {
            g_flag_follow = 0;
            g_flag_avoid = 0;
        }
        if (count > 180 && count < 360) /* 避障模式 */
        {
            g_flag_avoid = 1;
            g_flag_follow = 0;
        }
        if (count > 360 && count < 540) /* 跟随模式 */
        {
            g_flag_avoid = 0;
            g_flag_follow = 1;
        }
        if (count > 540)
            count = 0;
    }
}

#ifdef __cplusplus
}
#endif
