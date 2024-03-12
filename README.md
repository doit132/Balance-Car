# 1. 创建工程

使用 STM32CubeMX 创建一个 STM32F103C8T6 Makefile 工程, 添加一些基本文件, 例如 .clang-format, .gitignore, clang-format.py, build.sh 文件

- .clang-format 源码文件统一格式化规范
- build.sh 辅助编译脚本, 一键编译下载, openocd 下载调试
- .gitignore 版本管理文件
- clang-format.py 批量格式化源码脚本文件

# 2. 外设模块驱动编写

## 2.1 串口

### 2.1.1 串口 IO 资源分配

| IO 名称 | 说明     |
| ------- | -------- |
| PA9     | USART_TX |
| PA10    | USART_RX |

### 2.1.2 串口配置

STM32CubeMX 的默认配置, 波特率: 115200, 数据位长度: 8, 无校验

### 2.1.3 串口 printf 重定义

```c
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
```

注意: 上面的串口 printf 重定义是针对 GCC 编译器的, 并且上述的串口 printf 重定义无法输出浮点数, 为了输出浮点数还需要设置一个链接选项

打开 Makefile 文件, 然后做出以下修改:

```c
LDFLAGS = $(MCU) -specs=nano.specs -T $(LDSCRIPT) $ (LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
--> 在链接选项中增加 -u _printf_float, 添加后编译生成的文件增大 8KB
LDFLAGS = $(MCU) -specs=nano.specs -u _printf_float -T $(LDSCRIPT) $ (LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
```
