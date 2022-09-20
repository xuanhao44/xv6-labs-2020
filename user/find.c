// find.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *fmtname(char *path);

void find(char *path, char *name);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        // 缺参数
        fprintf(2, "usage: find path name\n");
        exit(1);
    }

    char *path = argv[1];
    char *name = argv[2];

    find(path, name);

    exit(0);
}

char *fmtname(char *path)
{
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    return p;
}

void find(char *path, char *name)
{
    char buf[512], *p; // 完整目录存储的字符数组（大概），p 是用来操作这个数组的指针
    int fd;
    struct dirent de; // 目录项
    struct stat st;   // 存储文件信息的结构体

    // 尝试按照路径打开文件
    if ((fd = open(path, 0)) < 0)
    {
        // 打不开就算了
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    // 尝试查看文件信息
    if (fstat(fd, &st) < 0)
    {
        // 查不到就算了
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        // 文件
        if (strcmp(fmtname(path), fmtname(name)) == 0)
        {
            printf("%s\n", path);
        }
        break;

    case T_DIR:
        // 目录
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            // path 目录的基础上查看目录里的文件还要加上 '/' + 一个目录项 + '/' （大概）
            // 加上这就超出 512 了就说明 path 太长了
            printf("find: path too long\n");
            break;
        }

        // 先复制一份 path 到 buf
        strcpy(buf, path);
        // 操作指针 p 也指向字符串的末尾
        p = buf + strlen(buf);
        // 补上一个 / 然后右移一位
        *p++ = '/';

        // 从 fd 里读目录项
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            // 没内容就跳过
            if (de.inum == 0)
                continue;

            // 拒绝循环递归
            int judge = strcmp(de.name, ".") && strcmp(de.name, "..");
            if (judge == 0)
                continue;

            // 有内容就把目录项名字的字符串复制到 buf 中（通过指针 p）
            // 可以发现，复制好像把空格也复制进去了
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            // 剩下的不管是文件还是目录要 find
            find(buf, name);
        }
        break;
    }
    close(fd);
}
