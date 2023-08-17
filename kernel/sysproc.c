#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
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
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
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

  if(argint(0, &pid) < 0)
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

//增加系统调用
uint64
sys_trace(void)
{
  int mask;

  argint(0, &mask);
  struct proc *p = myproc();
  p->mask = mask;
  return 0;
}

// 定义一个函数，用于向用户提供系统信息
uint64
sys_sysinfo(void)
{
  // 用户空间指向 sysinfo 结构体的指针
  uint64 si_addr;

  // 获取系统调用参数，并将用户空间指针保存到 si_addr 变量中
  argaddr(0, &si_addr);

  int nproc;
  int freemem;

  // 获取未使用进程数量和可用内存数量
  nproc = proc_not_unsed_num();
  freemem = free_mem_num();

  // 创建一个 sysinfo 结构体对象并填充信息
  struct sysinfo sysinfo;
  sysinfo.freemem = freemem;
  sysinfo.nproc = nproc;

  // 获取当前进程的指针
  struct proc *p = myproc();
  
  // 将 sysinfo 结构体拷贝到用户空间指定地址
  if (copyout(p->pagetable, si_addr, (char *)&sysinfo, sizeof(sysinfo)) < 0)
    return -1;

  return 0;
}