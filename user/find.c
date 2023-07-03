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

void find(char* path,char* targetName){

  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
  

  switch(st.type){
    case T_FILE:
      //printf("当前路径是普通文件，文件名称为：%s,targetName是%s\n",fmtname(path),targetName);
      //printf("strcmp(fmtname(path),targetName)==%d\n",strcmp(fmtname(path),targetName));
      if(strcmp(fmtname(path),targetName)==32){
        printf("%s\n",path);
      }
      break;
    case T_DIR:
        // if(strcmp(fmtname(path),".")==0||strcmp(fmtname(path),"..")==0)
        //   {
        //     printf("遇到.或者..，break\n");
        //     break;
        //   }
        
        //printf("进入目录遍历\n");
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
          printf("ls: path too long\n");
          break;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
          if((de.inum == 0) || (strcmp(de.name, ".") == 0) || (strcmp(de.name, "..") == 0)){
              continue;
          }
          memmove(p, de.name, DIRSIZ);
          p[DIRSIZ] = 0;
          if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
          }
          find(buf,targetName);
        } 
      break;

  }
  close(fd);
}


int
main(int argc, char *argv[])
{
  
  if(argc!=3){
    printf("参数数量有误，请重新输入命令！\n");
    exit(0);
  }
  find(argv[1],argv[2]);

  exit(0);
}
