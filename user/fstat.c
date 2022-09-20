// fstat.c

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

int main()
{
    char path[512] = "./.";

    struct stat st;

    // 获取文件信息
    int flag = stat(path, &st);

    printf("flag = %d\n", flag);
    printf("type: %d, inode number: %d, size: %d\n", st.type, st.ino, st.size);

    exit(0);
}
