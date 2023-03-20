// final code:sleep.c
// 调用system call sleep来实现休眠一定时间，注意如果没有传入参数，程序需要打印错误信息
#include "kernel/types.h"
#include "user/user.h" //声明int sleep(int);

int main(int argc, char *argv[])
{
    if (argc == 1)
    { // 如果传入的参数只有一个 sleep 指令
        fprintf(2, "ERROR: sleep time required\n");
        exit(1);
    }
    sleep(atoi(argv[1])); // atoi把字符串转化为int,sleep的定义在内核 proc.c 的第555行
    exit(0);
}