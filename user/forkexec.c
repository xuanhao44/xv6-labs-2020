// forkexec.c

// 演示fork/exec程序。

#include "kernel/types.h"
#include "user/user.h"

int main()
{
    int pid, status;

    pid = fork();

    if (pid == 0)
    {
        char *argv[] = {"echo", "this", "is", "echo", 0};

        exec("echo", argv);
        // 这里写的程序必须存在，不然就会出错
        // 出错后就返回父进程了
        printf("exec failed!\n");
        exit(1);
    }
    else
    {
        int ret;

        printf("parent waiting\n");

        // wait的参数status，
        // 是一种让退出的子进程以一个整数（32bit的数据）的格式与等待的父进程通信方式。
        ret = wait(&status);
        // wait 还返回了子进程的进程号

        printf("the child, which pid = %d, exited with status %d\n", ret, status);
    }

    exit(0);
}
