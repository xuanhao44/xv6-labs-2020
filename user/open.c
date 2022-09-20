// open.c

// 创建一个叫做output.txt的新文件，并向它写入一些数据，最后退出。
// 我们看不到任何输出，因为它只是向打开的文件中写入数据。

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main()
{
    int fd = open("output.txt", O_WRONLY | O_CREATE);
    write(fd, "hello friend!\n", 14);

    exit(0);
}
