// primes.c

#include "kernel/types.h"
#include "user/user.h"

void child(int *p)
{
    // 关闭上一个进程的管道的写端口
    close(p[1]);

    int i;
    int read_r = read(p[0], &i, sizeof(int));

    if (read_r == 0)
    {
        // 没有数被传到这个进程来 该结束了
        close(p[0]);
        exit(0);
    }
    else if (read_r > 0)
    {
        // 每次，或者说每个子进程只打印当前那个最小的数，也即那个素数
        printf("prime %d\n", i);
    }
    else
    {
        // -1
        printf("get wrong!\n");
        exit(1);
    }

    // 先创建管道，关闭读端口
    int temp_p[2];
    pipe(temp_p);

    if (fork() == 0)
    {
        // 子进程 递归
        child(temp_p);
    }
    else
    {
        // 当前进程

        // 首先关闭与下一个进程交流的管道的读端口
        close(temp_p[0]);

        int j;
        while ((read_r = read(p[0], &j, sizeof(int))) > 0)
        {
            // 如果不会被整除，就把它塞到新管道中。让下一个进程处理
            if (j % i != 0)
            {
                write(temp_p[1], &j, sizeof(int));
            }
        }
        // 小于 0 就有问题，等于 0 就是正常的（上面已经输出完最后一个数了）
        if (read_r < 0)
        {
            // -1
            printf("get wrong!\n");
            exit(1);
        }

        // 最后关闭 p[0] 读端口，也关闭 temp_p[1] 写端口
        close(p[0]);
        close(temp_p[1]);

        // 等待，等到了子进程返回，自己也结束了
        wait(0);
        exit(0);
    }
}

int main()
{

    int p[2];
    pipe(p);

    if (fork() == 0)
    {
        child(p);
    }
    else
    {
        close(p[0]);

        for (int i = 2; i <= 35; i++)
        {
            write(p[1], &i, sizeof(int));
        }
        // 写完就要关掉!
        close(p[1]);

        wait(0);
    }

    exit(0);
}
