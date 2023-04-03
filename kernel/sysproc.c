#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h" // 添加 sysinfo.h 头文件

uint64
sys_exit(void)
{
  int n;
  if (argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if (argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_trace(void) // 在这里添加 系统调用 sys_trace
{
  int n; // 定义一个 n 用于保存掩码

  if (argint(0, &n) < 0) // 获取掩码
    return -1;
  myproc()->syscallnum = n; // 将掩码保存到 proc.h 中的结构体 proc 中的新定义变量 syscallnum 中
  return 0;
}

uint64
sys_sysinfo(void) // 在这里添加 系统调用 sys_sysinfo
{
  uint64 sysinfop; // 系统信息结构指针的存放地址
  struct sysinfo si;

  if (argaddr(0, &sysinfop) < 0)//函数的定义在 kernel/syscall.c中 读取a0寄存器的数据放到先前定义的sysinfop中 
    return -1;
  si.freemem = freememsize(); // 获取空闲内存的大小，在kernel/kalloc.c中实现
  si.nproc = nproc_active();  // 获取没有用到的进程的数量，在kernel/proc.c中实现
  if (copyout(myproc()->pagetable, sysinfop, (char *)&si, sizeof(si)) < 0)
    // 将sysinfo struct si从内核空间复制到用户空间 ,可以参考 kernel/vm.c 
    return -1;
  return 0;
}
