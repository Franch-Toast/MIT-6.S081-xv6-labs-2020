#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0)
  { 
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){// fstat()用来将参数fildes所指的文件状态，复制到参数buf所指的结构中(struct stat),执行成功则返回0，失败返回-1，错误代码存于errno
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  /*结构体解释说明如下：
  struct stat {
    int dev;     // File system's disk device
    uint ino;    // Inode number
    short type;  // Type of file
    short nlink; // Number of links to file
    uint64 size; // Size of file in bytes
  };

  又有：
  #define T_DIR     1   // Directory
  #define T_FILE    2   // File
  #define T_DEVICE  3   // Device
  */

  switch(st.type){
  case T_FILE:  //如果 path 是具体的某个文件，则打印该文件的信息
    printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR: // 如果 path 是某个文件夹
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){ 
      //检查原路径 + 1（\）+ 文件的命名长度（14） + 1（\）是否超过 512 字符
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);//把原路径拷贝一份给 buf
    p = buf+strlen(buf);//字符型指针指向 buf 的最后一个字符的位置
    *p++ = '/';// p 后移一位并添加 / 字符
    while(read(fd, &de, sizeof(de)) == sizeof(de))
    { // 从原路径文件夹读取并填充 char name[DIRSIZ];  DIRSIZ==14
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){//即指令为 ls
    ls(".");//显示当前目录下的所有文件
    exit(0);
  }
  for(i=1; i<argc; i++)
    ls(argv[i]); // 显示argv[]数组中的所有路径下的文件
  exit(0);
}
