// fork.c

// 使用fork的一个简单用例。

#include "kernel/types.h"
#include "user/user.h"

int main()
{
    int pid;

    // 调用 fork, 拷贝当前进程的内存，并创建一个新的进程
    pid = fork();

    // fork系统调用在两个进程中都会返回，
    // 在原始的进程中，fork系统调用会返回大于0的整数，这个是新创建进程的ID。
    // 而在新创建的进程中，fork系统调用会返回0。

    // 每次的输出不一定一样，看起来像是垃圾数据。
    // 两个进程实际上就是同时在运行。
    // 所以当这两个进程在输出的时候，它们会同时一个字节一个字节的输出，两个进程的输出交织在一起。
    // printf("fork() return %d\n", pid);

    if (pid == 0)
    {
        // fork 返回值是 0 的就是新创建的进程，这样来判断
        printf("child: PID = %d\n", getpid());
    }
    else
    {
        printf("parent: PID = %d\n", getpid());
    }

    exit(0);
}
