/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "soft_timer.h"
#include "delay.h"
#include "usart.h"
#include "bsp.h"
#include "control.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1)
    {
    }
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
    /* USER CODE BEGIN SVCall_IRQn 0 */

    /* USER CODE END SVCall_IRQn 0 */
    /* USER CODE BEGIN SVCall_IRQn 1 */

    /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
    /* USER CODE BEGIN PendSV_IRQn 0 */

    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */

    /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */
    SysTick_ISR();
    Systick_Delay_ISR();
    /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
    /* USER CODE BEGIN TIM3_IRQn 0 */

    /* USER CODE END TIM3_IRQn 0 */
    HAL_TIM_IRQHandler(&htim3);
    /* USER CODE BEGIN TIM3_IRQn 1 */

    /* USER CODE END TIM3_IRQn 1 */
}

/* USER CODE BEGIN 1 */

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
// {
//     extern u8 g_rx_buf[20];
//     if (huart == &huart1) /* 串口 1 接收到 PC 发来的数据 */
//     {
//         HAL_UART_Transmit_IT(&huart3, g_rx_buf, 20);
//         HAL_UART_Transmit_IT(&huart1, g_rx_buf, 20);
//         HAL_UART_Receive_IT(&huart1, g_rx_buf, 20);
//     }
//     if (huart == &huart3) /* 串口 3 接收到蓝牙模块发送来的数据 */
//     {
//         HAL_UART_Transmit_IT(&huart1, g_rx_buf, 20);
//         HAL_UART_Receive_IT(&huart3, g_rx_buf, 20);
//     }
// }

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size)
{
    extern u8 g_rx_buf[20];
    if (huart == &huart1) /* 串口 1 接收到 PC 发来的数据 */
    {
        // HAL_UART_Transmit_IT(&huart3, g_rx_buf, Size);
        HAL_UART_Transmit_IT(&huart1, g_rx_buf, Size);
        HAL_UARTEx_ReceiveToIdle_IT(&huart1, g_rx_buf, 20);
    }
    if (huart == &huart3) /* 串口 3 接收到蓝牙模块发送来的数据 */
    {
        HAL_UART_Transmit_IT(&huart1, g_rx_buf, Size);
        HAL_UARTEx_ReceiveToIdle_IT(&huart3, g_rx_buf, 20);
    }
}

// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
// {
//     // static int Voltage_Temp, Voltage_Count, Voltage_All; // 电压测量相关变量
//     static u8 Flag_Target; // 控制函数相关变量，提供10ms基准
//     // int        Encoder_Left, Encoder_Right;              // 左右编码器的脉冲计数
//     int Balance_Pwm, Velocity_Pwm, Turn_Pwm; // 平衡环PWM变量，速度环PWM变量，转向环PWM变
//     int Motor_Left, Motor_Right;
//     int cnt;
//     if (GPIO_Pin == MPU6050_INT_Pin)
//     {
//         MPU6050_DMP_Get_Data(&g_pitch, &g_roll, &g_yaw);
//         Balance_Pwm = Control_Balance_PD(g_pitch, 0, g_gyro_y); // 平衡PID控制
//         Motor_Left = Balance_Pwm;
//         Motor_Right = Balance_Pwm;
//         // PWM值正数使小车前进，负数使小车后退
//         Motor_Left = PWM_Limit(Motor_Left, 6900, -6900);
//         Motor_Right = PWM_Limit(Motor_Right, 6900, -6900); // PWM限幅
//         if (Check_Exception() == 0)                        // 如果不存在异常
//             Motor_Set_Pwm(Motor_Left, Motor_Right);        // 赋值给PWM寄存器
//         cnt = Systick_GetTick();
//         printf("cnt=%d\r\n", cnt);
//     }
// }

/* USER CODE END 1 */
