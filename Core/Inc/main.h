/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* ANCHOR - 全局变量声明 */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin               GPIO_PIN_4
#define LED_GPIO_Port         GPIOA
#define KEY_Pin               GPIO_PIN_5
#define KEY_GPIO_Port         GPIOA
#define VOL_Pin               GPIO_PIN_6
#define VOL_GPIO_Port         GPIOA
#define ECHO_Pin              GPIO_PIN_0
#define ECHO_GPIO_Port        GPIOB
#define TRIG_Pin              GPIO_PIN_1
#define TRIG_GPIO_Port        GPIOB
#define BT_TX_Pin             GPIO_PIN_10
#define BT_TX_GPIO_Port       GPIOB
#define BT_RX_Pin             GPIO_PIN_11
#define BT_RX_GPIO_Port       GPIOB
#define BIN2_Pin              GPIO_PIN_12
#define BIN2_GPIO_Port        GPIOB
#define BIN1_Pin              GPIO_PIN_13
#define BIN1_GPIO_Port        GPIOB
#define AIN1_Pin              GPIO_PIN_14
#define AIN1_GPIO_Port        GPIOB
#define AIN2_Pin              GPIO_PIN_15
#define AIN2_GPIO_Port        GPIOB
#define PWM_Right_Pin         GPIO_PIN_8
#define PWM_Right_GPIO_Port   GPIOA
#define PWM_Left_Pin          GPIO_PIN_11
#define PWM_Left_GPIO_Port    GPIOA
#define MPU6050_INT_Pin       GPIO_PIN_12
#define MPU6050_INT_GPIO_Port GPIOA
#define OLED_DC_Pin           GPIO_PIN_15
#define OLED_DC_GPIO_Port     GPIOA
#define OLED_RES_Pin          GPIO_PIN_3
#define OLED_RES_GPIO_Port    GPIOB
#define OLED_SDA_Pin          GPIO_PIN_4
#define OLED_SDA_GPIO_Port    GPIOB
#define OLED_SCL_Pin          GPIO_PIN_5
#define OLED_SCL_GPIO_Port    GPIOB
#define MPU6050_SCL_Pin       GPIO_PIN_8
#define MPU6050_SCL_GPIO_Port GPIOB
#define MPU6050_SDA_Pin       GPIO_PIN_9
#define MPU6050_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
