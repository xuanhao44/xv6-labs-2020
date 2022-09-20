// copy.c

// 它会等待输入。我随便输入一些字符，程序会读取我输入的字符，并将相同的字符输出给我。

#include "kernel/types.h"
#include "user/user.h"

// 这个程序里面执行了3个系统调用，分别是read，write和exit。

int main()
{
    char buf[64];

    while (1)
    {
        // fd, 0 means stdin
        // 指向某段内存的指针，程序可以通过指针对应的地址读取内存中的数据
        // 代码想读取的最大长度，sizeof(buf)表示，最多读取64字节的数据
        int n = read(0, buf, sizeof(buf));
        // 返回值: 正常情况: 读到的字节数(加上了结束符)
        // 出现了错误可能会返回 -1
        if (n <= 0)
            break;
        write(1, buf, n);
        // fd, 1 means stdout
    }

    // 最后一定 exit
    exit(0);
}
