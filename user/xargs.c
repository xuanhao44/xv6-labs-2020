#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

// ulib.c 的 get 改造而来
char *ugets(char *buf, int max)
{
    int i, cc;
    char c;

    for (i = 0; i + 1 < max;)
    {
        cc = read(0, &c, 1);
        if (cc < 1)
            break;
        // 在遇到 \n \r 和 blank 时停止循环
        if (c == '\n' || c == '\r' || c == ' ')
            break;
        // 不把 \n \r 和 blank 读到字符串中
        buf[i++] = c;
    }
    buf[i] = '\0';
    return buf;
}

// sh.c 的 getcmd 改造而来
int ugetcmd(char *buf, int nbuf)
{
    memset(buf, 0, nbuf);
    ugets(buf, nbuf);
    if (buf[0] == 0) // EOF
        return -1;
    return 0;
}

int main(int argc, char *argv[])
{
    /*
    argv[0] = "xargs"
    argv[1] = command
    argv[2] = para
    ...
    argv[END] = 0
    */

    if (fork() == 0)
    {
        // 还是在子进程里 exec 吧
        /*
        获得新参数列表
        之后就使用 exec(argv_alter[0], argv_alter);
        */
        char *argv_alter[MAXARG];
        int argc_alter = argc - 1;

        for (int i = 0; i < argc_alter; i++)
            argv_alter[i] = argv[i + 1];

        char buf[100];

        while (ugetcmd(buf, sizeof(buf)) >= 0)
        {
            // buf 一直在变，不能添加到字符串指针数组里
            // 故使用临时的 temp
            char temp[100];
            strcpy(temp, buf);
            argv_alter[argc_alter] = temp;
            argc_alter++;
        }
        argv_alter[argc_alter] = 0;

        // exec 执行
        exec(argv_alter[0], argv_alter);
        // 失败了就退出了
        printf("exec failed!\n");
        exit(1);
    }
    else
    {
        wait(0);
    }
    exit(0);
}
