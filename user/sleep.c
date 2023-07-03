#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int
main(int argc, char *argv[])
{
  if(argc!=2){
    printf("输入参数数量有误，请重新输入命令！\n");
    exit(-1);
  }
  int n=atoi(argv[1]);
  sleep(n);

  exit(0);
}
