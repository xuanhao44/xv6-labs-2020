#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

// ulib.c 的 get 改造而来
char *ugets(char *buf, int max);

// sh.c 的 getcmd 改造而来
int ugetcmd(char *buf, int nbuf);

// 向字符指针数组后面添加一个指针
void append1(int *argc, char *argv[], char *token);

int main(int argc, char *argv[])
{
    /*
    argv[0] = "xargs"
    argv[1] = command
    argv[2] = para
    ...
    argv[END] = 0
    */

    char *argv_alter[MAXARG];
    int argc_alter = argc - 1;

    for (int i = 0; i < argc_alter; i++)
        argv_alter[i] = argv[i + 1];

    // buf 是从标准输入读数据的缓冲区
    char buf[100];
    // token 是实际存这些输入的空间
    char token[MAXARG][100];

    int len = 0; // token 长度
    while (ugetcmd(buf, sizeof(buf)) >= 0)
        strcpy(token[len++], buf); // buf 一直在变，不能添加到字符串指针数组里

    for (int i = 0; i < len; i++)
    {
        if (fork() == 0)
        {
            // -n 1
            append1(&argc_alter, argv_alter, token[i]);
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
    }

    exit(0);
}

// ulib.c 的 gets 改造而来
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

// 向字符指针数组后面添加一个指针
void append1(int *argc, char *argv[], char *token)
{
    argv[*argc] = token;
    (*argc)++;
    argv[*argc] = 0;
}
