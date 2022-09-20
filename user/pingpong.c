// pingpong.c

#include "kernel/types.h"
#include "user/user.h"

int main()
{
    // 父进程向子进程传 ping，子进程向父进程传 pong
    // 我希望先显示子进程 ping，后显示父进程 pong
    // 若不使用 wait，就需要利用管道的读写阻塞

    int p[2];
    pipe(p);

    int pid = fork();

    char buf[64];

    int status;

    if (pid == 0)
    {
        /* 子进程 */

        read(p[0], buf, 4);
        close(p[0]);

        printf("%d: received %s\n", getpid(), buf);

        write(p[1], "pong", 4);
        close(p[1]);
    }
    else
    {
        /* 父进程 */

        write(p[1], "ping", 4);
        close(p[1]);

        wait(&status);

        read(p[0], buf, 4);
        close(p[0]);

        printf("%d: received %s\n", getpid(), buf);
    }

    exit(0);
}
