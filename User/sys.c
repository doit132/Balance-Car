#ifdef __cplusplus
extern "C" {
#endif

/* ANCHOR - 头文件包含 */

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include "sys.h"

int _isatty(int fd);
int _write(int fd, char* ptr, int len);
int _close(int fd);
int _lseek(int fd, int ptr, int dir);
int _read(int fd, char* ptr, int len);
int _fstat(int fd, struct stat* st);

/* ANCHOR - 公共函数定义 */

/* NOTE - 为了消除编译警告 */

__attribute__((weak)) int _isatty(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 1;

    errno = EBADF;
    return 0;
}

__attribute__((weak)) int _close(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 0;

    errno = EBADF;
    return -1;
}

__attribute__((weak)) int _lseek(int fd, int ptr, int dir)
{
    (void)fd;
    (void)ptr;
    (void)dir;

    errno = EBADF;
    return -1;
}

__attribute__((weak)) int _fstat(int fd, struct stat* st)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    {
        st->st_mode = S_IFCHR;
        return 0;
    }

    errno = EBADF;
    return 0;
}

__attribute__((weak)) int _read(int file, char* ptr, int len)
{
    (void)file;
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        *ptr++ = getchar();
    }
    return len;
}

__attribute__((weak)) int _write(int file, char* ptr, int len)
{
    (void)file;
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        putchar(*ptr++);
    }
    return len;
}

__attribute__((weak)) void _getpid(void)
{
}

__attribute__((weak)) void _kill(void)
{
}

#ifdef __cplusplus
}
#endif
