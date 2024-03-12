/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{
    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (uartHandle->Instance == USART1)
    {
        /* USER CODE BEGIN USART1_MspInit 0 */

        /* USER CODE END USART1_MspInit 0 */
        /* USART1 clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN USART1_MspInit 1 */

        /* USER CODE END USART1_MspInit 1 */
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
    if (uartHandle->Instance == USART1)
    {
        /* USER CODE BEGIN USART1_MspDeInit 0 */

        /* USER CODE END USART1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

        /* USER CODE BEGIN USART1_MspDeInit 1 */

        /* USER CODE END USART1_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* ANCHOR - gcc printf 重定义 */

#if 1 /* printf 重定义 */
    #ifdef __GNUC__
        #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
    #else
        #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE* f)
    #endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
    /* 实现串口发送一个字节数据的函数 */
    while ((USART1->SR & 0x40) == 0)
    {
    }
    USART1->DR = (uint8_t)ch;
    return ch;
}

int _write(int file, char* ptr, int len)
{
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        __io_putchar(*ptr++);
    }
    return len;
}
#endif /* printf 重定义 */

/* Start - Test 测试函数 */

/* NOTE - 私有测试函数定义 */

/**
 * *****************************************************************************
 * @brief 测试 MCU 串口 1 使用 HAL_UART_Transmit 函数发送数据
 * *****************************************************************************
 */
static void Test_USART1_Transmit(void)
{
    char buf[20] = "hello world\r\n";

    while (1)
    {
        /* 1. 发送字符 ------------------------------------------------------------------ */
        // HAL_UART_Transmit(&huart1, (uint8_t*)buf, 20, 1000);

        /* 2. printf 串口重定义测试, 打印字符 ------------------------------------------------ */
        /* 记得 在 usart.h 文件中添加 stdio.h 头文件 */
        // printf("hello world\r\n");

        /* 3. printf 串口重定义测试, 打印整数 -------------------------------------------------- */
        // printf("10 + 20 = %d\r\n", 10 + 20);

        /* 4. printf 串口重定义测试, 打印浮点数 ------------------------------------------------- */
        /* 默认的串口 printf 重定义, 无法打印浮点数, 需要在链接选项中添加 -u _printf_float */
        /**
         * 4.1 打开 Makefile 文件
         * 4.2 LDFLAGS = $(MCU) -specs=nano.specs -u _printf_float -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections 
         */
        printf("10.0 + 30.0 = %.2f\r\n", 10.0 + 30.0);
        HAL_Delay(1000);
    }
}

/* NOTE - 公共测试函数定义 */

/**
 * *****************************************************************************
 * @brief 串口 1 的主测试函数, 供 main 函数调用
 * *****************************************************************************
 */
void Test_USART1(void)
{
    Test_USART1_Transmit();
}

/* End - Test 测试函数 */

/* USER CODE END 1 */
