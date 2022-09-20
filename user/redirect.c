// redirect.c

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main()
{
    int pid;

    pid = fork();

    int status;

    if (pid == 0)
    {
        close(1);
        // 先关闭标准输出 stdout, 1 空出

        open("output.txt", O_WRONLY | O_CREATE);
        // 文件描述符1与文件output.txt关联。

        char *argv[] = {"echo", "this", "is", "redirected", "echo ", 0};

        exec("echo", argv);

        printf("exec failed!\n");
        exit(1);
    }
    else
    {
        wait(&status);
        printf("the child exec axwith exit %d!\n", status);
    }

    exit(0);
}
