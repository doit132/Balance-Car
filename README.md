# 1 创建工程

使用 STM32CubeMX 创建一个 STM32F103C8T6 Makefile 工程, 添加一些基本文件, 例如 .clang-format, .gitignore, clang-format.py, build.sh 文件

- .clang-format 源码文件统一格式化规范
- build.sh 辅助编译脚本, 一键编译下载, openocd 下载调试
- .gitignore 版本管理文件
- clang-format.py 批量格式化源码脚本文件

# 2 外设模块驱动编写

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
# if3 /* printf 重定义 */
# 4 ifdef __GNUC__
# 5 define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
# 6 else
# 7 define PUTCHAR_PROTOTYPE int fputc(int ch, FILE* f)
# 8 endif /* __GNUC__ */

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
# 9 endif /* printf 重定义 */
```

注意: 上面的串口 printf 重定义是针对 GCC 编译器的, 并且上述的串口 printf 重定义无法输出浮点数, 为了输出浮点数还需要设置一个链接选项

打开 Makefile 文件, 然后做出以下修改:

```c
LDFLAGS = $(MCU) -specs=nano.specs -T $(LDSCRIPT) $ (LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
--> 在链接选项中增加 -u _printf_float, 添加后编译生成的文件增大 8KB
LDFLAGS = $(MCU) -specs=nano.specs -u _printf_float -T $(LDSCRIPT) $ (LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
```

## 2.2 LED

### 2.2.1 LED 原理图

![1710241757491](images/1710241757491.png)

原理图分析:

- 当 PA4 输出低电平的时候, LED 亮
- 当 PA4 输出高电平的时候, LED 灭
- 让 PA4 的初始状态为高电平

### 2.2.2 LED IO 资源分配

| IO 口 | 说明 |
| ----- | ---- |
| PA4   | LED  |

### 2.2.3 LED GPIO 口配置

推挽输出, 默认高电平 (LED 灭), 速率低

## 2.3 Key 按键

### 2.3.1 Key 按键原理图

![1710259733535](images/1710259733535.png)

原理图分析:

- 按键按下, PA5 为低电平
- 按键释放, PA5 为高电平

### 2.3.2 Key IO 资源分配

| IO 口 | 说明     |
| ----- | -------- |
| PA5   | 用户按键 |

### 2.3.3 Key GPIO 配置

浮空输入, 由于外部有上拉电阻存在, 所以默认 PA5 是高电平

### 2.3.4 按键扫描, 检测按键单击, 双击, 长按

为了不让按键影响其他任务的执行, 需要使用非阻塞的方式进行按键扫描

非阻塞有三种实现方式:

1. while(1) 循环作为时基 (受任务数量, 时基不确定)
2. 定时器作为时基, 使用周期性定时器中断来检测按键 (时基确定)
3. 按键外部中断触发, 定时器辅助

按键扫描有2种判断方式: 

1. 状态机 (逻辑清晰)
2. 条件判断+标志位 (逻辑复杂, 需要很多条件判断和标志位变量)

选择**软件定时器+状态机**的组合: 详情请见源码
