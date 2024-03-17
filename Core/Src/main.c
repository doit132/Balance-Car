/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "test.h"
#include "show.h"
#include "encoder.h"

/* 外设头文件 */

#include "bsp.h"
#include "soft_timer.h"

#include "task.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DATA_MAX 20
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* ANCHOR - 公共全局变量 */

u8             g_way_angle = 1;               /* 获得欧拉角的方式: 1: DMP 2: 卡尔曼滤波 3: 互补滤波 */
float          g_balance_kp = 650 * 0.6;      /* 直立环 PD 控制器 比例控制系数 */
float          g_balance_kd = 30 * 0.6;       /* 直立环 PD 控制器 微分控制系数 */
float          g_velocity_kp = 220.0;         /* 速度环 PD 控制器 比例控制系数 */
float          g_velocity_ki = 220.0 / 200.0; /* 速度环 PI 控制器 积分控制系数 */
u8             g_flag_stop = 1;               /* 小车停止标志位 */
float          g_voltage;                     /* 电池电压 */
u8             g_flag_avoid;                  /* 小车避障模式标志位 */
u8             g_flag_follow;                 /* 小车跟随模式标志位 */
Hcsr04Info_t   g_hcsr04_info;                 /* 超声波传感器结构体, 内部有测量距离 */
Encoder_Data_t g_encoder_data;                /* 编码器数据 */
Motor_Data_t   g_motor_data;                  /* 电机数据 */
u8             g_key_num;                     /* 0: 无按键, 1: 短按, 2: 长按, 3: 双击 */
u8             g_rx_buf[DATA_MAX];            /* 串口接收缓冲区 */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_TIM3_Init();
    MX_I2C1_Init();
    MX_ADC1_Init();
    MX_TIM2_Init();
    MX_TIM4_Init();
    MX_TIM1_Init();
    MX_USART3_UART_Init();
    /* USER CODE BEGIN 2 */
    SoftTimer_Init(); /* 需要在外设初始化之前运行, 因为有的外设初始化需要软件定时器 */
    BSP_Init();

    SoftTimer_Start(0, 5, SOFT_TIMER_MODE_PERIODIC, "Data Collect"); /* 5ms 一个周期 */
    SoftTimer_Start(1, 200, SOFT_TIMER_MODE_PERIODIC, "Data Show");  /* 200ms 一个周期 */
    SoftTimer_Start(2, 10, SOFT_TIMER_MODE_PERIODIC, "PID Control"); /* 10ms 一个周期 */
    // HAL_UARTEx_ReceiveToIdle_IT(&huart1, g_rx_buf, DATA_MAX);
    // HAL_UARTEx_ReceiveToIdle_IT(&huart3, g_rx_buf, DATA_MAX);
    u64 cnt_start, cnt_end;
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        // Test_USART1();
        // Test_LED();
        // Test_SoftTimer();
        // Test_Key();
        // Test_HCSR04();
        // Test_OLED();
        // Test_MPU6050();
        // Test_ADC();
        // while (1)
        // {
        //     g_voltage = ADC_Get_Battery_Volt();
        //     OLED_Show();
        //     HAL_Delay(100);
        // }
        // Test_Motor();

        if (SoftTimer_Check(0))
        {
            // cnt_start = Systick_GetTick();
            Task_Data_Collect();
            Task_Encoder_Control();
            Task_Key_Control();
            if (g_key_num == 2)
            {
                LED_Flash(500, 2);
            }
            // cnt_end = Systick_GetTick();
            // printf("cnt = %d\r\n", (int)(cnt_end - cnt_start));
            // printf("gyrox = %f, gyroy = %f, gyroz = %f\r\n", g_gyro_x, g_gyro_y, g_gyro_z);
            // printf("accx = %f, accy = %f, accz = %f\r\n", g_acc_x, g_acc_y, g_acc_z);
            // printf("pitch = %f, roll = %f, yaw = %f\r\n", g_pitch, g_roll, g_yaw);
        }
        if (SoftTimer_Check(1))
        {
            cnt_start = Systick_GetTick();
            Task_Data_Show();
            cnt_end = Systick_GetTick();
            printf("cnt = %d\r\n", (int)(cnt_end - cnt_start));
        }
        if (SoftTimer_Check(2))
        {
            // cnt_start = Systick_GetTick();
            Task_Pid_Control();
            // cnt_end = Systick_GetTick();
            // printf("cnt = %d\r\n", (int)(cnt_end - cnt_start));
        }
        // cnt_end = Systick_GetTick();
        // printf("cnt = %d\r\n", (int)(cnt_end - cnt_start));

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef       RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef       RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
  */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
