// pipe_read.c

#include "kernel/types.h"
#include "user/user.h"

int main()
{
    int p[2];
    pipe(p);

    if (fork() == 0)
    {
        close(p[1]);

        int j;
        int read_r;
        while ((read_r = read(p[0], &j, sizeof(int))) > 0)
        {
            printf("prime %d\n", j);
        }
        // 小于 0 就有问题，等于 0 就是正常的
        if (read_r < 0)
        {
            printf("get wrong!\n");
            exit(1);
        }

        close(p[0]);
    }
    else
    {
        close(p[0]);

        for (int i = 2; i <= 35; i++)
        {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
    }

    exit(0);
}
