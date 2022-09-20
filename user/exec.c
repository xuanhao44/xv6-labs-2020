// exec.c

// 执行exec系统调用，这个系统调用会从指定的文件中读取并加载指令，并替代当前调用进程的指令。
// 从某种程度上来说，这样相当于丢弃了调用进程的内存，并开始执行新加载的指令。

#include "kernel/types.h"
#include "user/user.h"

int main()
{
    char *argv[] = {"echo", "this", "is", "echo", 0};
    // 0 标记了字符指针数组的结尾。

    // 操作系统从名为echo的文件中加载指令到当前的进程中，
    // 并替换了当前进程的内存，之后开始执行这些新加载的指令。
    exec("echo", argv);
    // 同时，你可以传入命令行参数，exec允许你传入一个命令行参数的数组

    printf("exec failed!\n");

    exit(0);
}
