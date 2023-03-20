#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];// 32 位数字字符掩码

  if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){ // 如果参数小于 3 或传入的字符掩码不正确（不是数字）
    fprintf(2, "Usage: %s mask command\n", argv[0]);      // 打印错误信息，"Usage: trace mask command\n"
    exit(1);
  }

  if (trace(atoi(argv[1])) < 0) {//atoi函数将argv[1]字符串转换为数字，并调用trace()，返回值小于 0 则打印错误信息
    fprintf(2, "%s: trace failed\n", argv[0]);
    exit(1);
  }
  
  for(i = 2; i < argc && i < MAXARG; i++){
    nargv[i-2] = argv[i];//掩码后面的参数全部填入nargv[]中
  }
  exec(nargv[0], nargv); // 以nargv为参数执行nargv[0]命令
  exit(0);
}
