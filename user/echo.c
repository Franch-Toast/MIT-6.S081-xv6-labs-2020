#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
//echo命令实际就是在将输入的参数重新写到输出上，即输入什么参数输出什么参数
int
main(int argc, char *argv[])
{
  int i;

  for(i = 1; i < argc; i++){
    write(1, argv[i], strlen(argv[i]));
    if(i + 1 < argc){//没有写入到最后一个参数，写入空格
      write(1, " ", 1);
    } else
    { // 写入到最后一个字符，写入换行
      write(1, "\n", 1);
    }
  }
  exit(0);
}
