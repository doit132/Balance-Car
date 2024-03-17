
/* ANCHOR - 头文件包含 */

#include "motor.h"
#include "tim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 宏定义 */

/* 表示电机的编号 */
#define LEFT_MOTOR  1
#define RIGHT_MOTOR 2
#define ALL_MOTOR   3

/* 表示电机的转动方向 */
#define FORWARD  1
#define BACKWARD 0

/* ANCHOR - 公共函数定义 */

/**
 * *****************************************************************************
 * @brief 电机初始化
 * @param [out] pData 存放电机数据的结构体
 * @note - 主要启动电机的 PWM 通道, 并将占空比设置为 0
 * *****************************************************************************
 */
void Motor_Init(Motor_Data_t* pData)
{
    HAL_TIM_PWM_Init(&htim1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, 0);

    pData->velocity_left = 0;
    pData->velocity_right = 0;
}

/**
 * *****************************************************************************
 * @brief 设置电机转向
 * @param [in] motor 控制哪一个电机
 * @param [in] direction 方向
 * @note - motor = 1 表示左电机, motor = 2 表示右电机
 * @note - direction = 1 表示正转 (顺时针, 前进), direction = 0 表示反转 (逆时针, 后退)
 * @note - 超声波测距模块面向自己, 以此参考系判断电机左右
 * *****************************************************************************
 */
static void Motor_Set_Direction(u8 motor, u8 direction)
{
    if (motor == 1) /* 左电机 */
    {
        if (direction == 1) /* 正转 (顺时针, 前进) */
        {
            HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
            return;
        }
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
        return;
    }

    if (direction == 1) /* 右电机 (顺时针, 前进) */
    {
        HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
        return;
    }
    HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
}

/**
 * *****************************************************************************
 * @brief 停止电机转动
 * *****************************************************************************
 */
void Motor_TurnOff(void)
{
    HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
}

/**
 * *****************************************************************************
 * @brief 给电机设置 PWM 占空比, 如果是负值后退, 如果是正值, 前进
 * @param [in] motor_left 0 表示停止, 负数表示后退, 正数表示前进
 * @param [in] motor_right 0 表示停止, 负数表示后退, 正数表示前进
 * @note - 超声波测距模块面向自己, 以此参考系判断电机左右
 * *****************************************************************************
 */
void Motor_Set_Pwm(int motor_left, int motor_right)
{
    if (motor_left > 0)
        Motor_Set_Direction(LEFT_MOTOR, FORWARD);
    else
        Motor_Set_Direction(LEFT_MOTOR, BACKWARD);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, SYS_Abs(motor_left));

    if (motor_right > 0)
        Motor_Set_Direction(RIGHT_MOTOR, FORWARD);
    else
        Motor_Set_Direction(RIGHT_MOTOR, BACKWARD);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, SYS_Abs(motor_right));
}

/**
 * *****************************************************************************
 * @brief 
 * @param [out] pMotorData 
 * @param [in] pEncoderData 
 * *****************************************************************************
 */
void Motor_Get_Velocity_From_Encoder(Motor_Data_t* pMotorData, Encoder_Data_t* pEncoderData)
{
    /* 转速=编码器读数(5ms 每次)*读取频率/倍频数/减速比/编码器精度 */

    /* 定义变量存储轮子速度, 单位: n/s (转/秒) */
    float Rotation_Speed_L, Rotation_Speed_R;

    /* 计算轮子转速 (n/s) */
    Rotation_Speed_L =
        pEncoderData->count_l * CONTROL_FREQUENCY / ENCODER_MULTIPLES / REDUCTION_RATIO / ENCODER_PRECISION;
    Rotation_Speed_R =
        pEncoderData->count_r * CONTROL_FREQUENCY / ENCODER_MULTIPLES / REDUCTION_RATIO / ENCODER_PRECISION;

    /* 计算轮子速度 (mm/s): 线速度 (m/s) = 转速 (n/s) * 周长 (mm) */
    pMotorData->velocity_left = Rotation_Speed_L * PI * DIAMETER_67;
    pMotorData->velocity_right = Rotation_Speed_R * PI * DIAMETER_67;
}

#ifdef __cplusplus
}
#endif
