// pipe.c

#include "kernel/types.h"
#include "user/user.h"

// 等效于 echo hello world | wc

int main()
{
    int p[2];
    pipe(p);
    // 正常创建后，p[1]为管道写入端，p[0]为管道读出端
    // 读出<-p[0]-p[1]<-写入

    /*
    打开文件就是 进程为文件分配一个文件描述符
    那这里 返回两个文件描述符，用以描述管道的读写端
    就可以认为是进程打开了管道的 读端文件和写端文件

    不用的话就要关掉，用完之后也要关掉
    */

    char *argv[] = {"wc", 0};

    int pid = fork();

    if (pid == 0)
    {
        // 子进程
        close(0);
        // 先关闭标准输入 stdin, 0 空出

        dup(p[0]);
        // 那么将管道的读端口拷贝在描述符0上

        close(p[0]);
        close(p[1]);
        // 关闭 p 中的描述符

        exec("wc", argv);
        // 执行 wc, 计算字数
    }
    else
    {
        // 父进程
        close(p[0]);
        // 关闭读端

        write(p[1], "hello world\n", 12);

        close(p[1]);
        // 写入完成，关闭写端
        wait(0);
    }

    exit(0);
}
