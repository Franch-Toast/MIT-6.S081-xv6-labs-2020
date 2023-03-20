#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h" //这里定义了最大参数量 #define MAXARG    32
#define MAX_LEN 100

int main(int argc, char *argv[])
{
    char *command = argv[1]; // 指向第二个参数
    char bf;
    char paramv[MAXARG][MAX_LEN]; // 参数存放的地方，是一个二维数组还是很令人神奇的，最大 32 行参数，每个参数最大长度 100
    char *m[MAXARG];

    while (1)
    {
        int count = argc - 1;                // # of current arguments，当前参数的数量
        memset(paramv, 0, MAXARG * MAX_LEN); // 全部填充为 0
        // copy the parameter of command
        for (int i = 1; i < argc; i++)
        { // 将原读取的命令行参数读取进 paramv 中，每一个参数读入不同的行
            strcpy(paramv[i - 1], argv[i]);
        }

        int cursor = 0; // cursor pointing the char position in single_arg
        int flag = 0;   // flag indicating whether there is argument preceding space
        int read_result;

        while (((read_result = read(0, &bf, 1))) > 0 && bf != '\n')
        { // 如果向标准输入中读取一个字符并存入 bf 成功 且读取到的结果不是 '\n' ，这是在读取第一个命令
            if (bf == ' ' && flag == 1)
            {            // 读入的字符是 ' ',且在此空格之前有参数
                count++; // 参数数量（指针） +1
                // reset flag and p
                cursor = 0;
                flag = 0;
            }
            else if (bf != ' ')
            {                                 // 读入的字符不是 ' '
                paramv[count][cursor++] = bf; // 填入参数数组
                flag = 1;                     // 标志位置 1 ，表明此前是填写过参数的
            }
        }
        // encounters EOF of input or \n
        if (read_result <= 0)
        { // 读取失败，直接跳出结束
            break;
        }
        for (int i = 0; i < MAXARG - 1; i++)
        { // 将读取到的所有参数都存入到 m 中
            m[i] = paramv[i];
        }
        m[MAXARG - 1] = 0; // m 最后补零，表明结束
        if (fork() == 0)
        {
            exec(command, m); // 子进程 运行指令及后续的参数
            exit(0);
        }
        else
        {
            wait((int *)0); // 等待子进程结束
        }
    }
    exit(0);
}