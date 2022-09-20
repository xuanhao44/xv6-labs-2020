#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// 这个程序的参数列表为echo xxxxx。
// 大部分的程序都忽略第一个参数，这个参数惯例上是程序的名字（这里是 echo）。

int main(int argc, char *argv[])
{
  int i;

  for (i = 1; i < argc; i++)
  {
    write(1, argv[i], strlen(argv[i]));
    if (i + 1 < argc)
    {
      write(1, " ", 1);
    }
    else
    {
      write(1, "\n", 1);
    }
  }
  exit(0);
}
