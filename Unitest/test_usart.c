#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include "test_usart.h"
#include "unity.h"
#include "sys.h"
#include "stm32f1xx_hal.h"

/* ANCHOR - 公共函数定义 */

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

void setUp(void)
{
    // 在这里配置东西
}

void tearDown(void)
{
    // 在这里清理东西
}

/**
 * *****************************************************************************
 * @brief 串口 1 的主测试函数, 供 main 函数调用
 * *****************************************************************************
 */
int Test_USART1(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_USART1_Transmit);
    return UNITY_END();
}

/* End - Test 测试函数 */

#ifdef __cplusplus
}
#endif
